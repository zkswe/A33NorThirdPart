/*
 * UartContext.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: guoxs
 */
#include "modbus/modbus.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <termio.h>
#include <sys/ioctl.h>

#include "uart/UartContext.h"
#include "uart/ProtocolSender.h"
#include "uart/ProtocolParser.h"
#include "utils/Log.h"
#include "ProtocolData.h"

static UINT baud_convert(UINT baudRate);
static int uart_trans_handle(SProtocolData *msg);

static SProtocolData *uartMessageTemp;
static modbus_t* mctx;

UartContext::UartContext() :
	mIsOpen(false),
	mUartID(0),
	mDataBufPtr(NULL),
	mDataBufLen(0) {

}

UartContext::~UartContext() {
	delete[] mDataBufPtr;
	closeUart();
}

bool UartContext::openUart(const char *pFileName, UINT baudRate) {
	UINT baud=0;

	baud = baud_convert(baudRate);
	LOGD("openUart pFileName = %s, baudRate = %d\n", pFileName, baud);
	mctx = modbus_new_rtu(pFileName, baud, 'N', 8, 1);
	modbus_set_slave(mctx, 1);
	modbus_connect(mctx);
	modbus_set_byte_timeout(mctx,0,100000);
	modbus_set_debug(mctx,1);

	mIsOpen = run("uart");
	if (!mIsOpen) {
		modbus_close(mctx);
	}

	LOGD("openUart mIsOpen = %d\n", mIsOpen);

	return mIsOpen;
}

void UartContext::closeUart() {
	LOGD("closeUart mIsOpen: %d...\n", mIsOpen);
	if (mIsOpen) {
		requestExit();

		modbus_close(mctx);
		mIsOpen = false;
	}
}

bool UartContext::send(const BYTE *pData, UINT len) {
	if (!mIsOpen) {
		return false;
	}

	if (write(mUartID, pData, len) != (int) len) {	// fail
		LOGD("send Fail\n");
		return false;
	}
//	LOGD("send Success\n");
	return true;
}

UartContext* UartContext::getInstance() {
	static UartContext sUC;
	return &sUC;           //UartContext类实例
}

bool UartContext::readyToRun() {
	
	//LOGD("uart readyToRun Success\n");

	return (1);
}

bool UartContext::threadLoop() {
	if (mIsOpen) {
		uartMessageTemp = uartMessageDeQueue();

		if(uartMessageTemp != (SProtocolData *)0)
		{
			int result = uart_trans_handle(uartMessageTemp);

			if(errno >= EMBBADCRC && errno <= EMBBADSLAVE && result == -1)
			{
				do
				{
                    modbus_close(mctx);
                    Thread::sleep(1000);
                    result = modbus_connect(mctx);
				}while(result == -1);   //重連串口
			}
		}
		else
		{
			Thread::sleep(20);
		}

		return true;
	}

	return false;
}

static UINT baud_convert(UINT baudRate)
{
	UINT baud = 0;

	switch(baudRate)
	{
		case 0000000: baud = 0;break;
		case 0000001: baud = 50;break;
		case 0000002: baud = 75;break;
		case 0000003: baud = 110;break;
		case 0000004: baud = 134;break;
		case 0000005: baud = 150;break;
		case 0000006: baud = 200;break;
		case 0000007: baud = 300;break;
		case 0000010: baud = 600;break;
		case 0000011: baud = 1200;break;
		case 0000012: baud = 1800;break;
		case 0000013: baud = 2400;break;
		case 0000014: baud = 4800;break;
		case 0000015: baud = 9600;break;
		case 0000016: baud = 19200;break;
		case 0000017: baud = 38400;break;
		case 0010001: baud = 57600;break;
		case 0010002: baud = 115200;break;
		default: baud = 9600;break;
	}

	return baud;
}

static int uart_trans_handle(SProtocolData *msg)
{
	int result = -1;

	switch(msg->cmd)
	{
		case TYPE_CMD_REG_READ:   //读寄存器
			result = modbus_read_registers(mctx, msg->reg, msg->nb, msg->data);
			break;
		case TYPE_CMD_REG_WRITE:  //写单个寄存器
			result = modbus_write_register(mctx, msg->reg, msg->data[0]);
			break;
		case TYPE_CMD_REG_WRITES: //写多个寄存器
			result = modbus_write_registers(mctx, msg->reg, msg->nb, msg->data);
			break;
		default: result = -1;break;
	}

	if(result < 0)
	{
		msg->err = errno;
	}
	parseProtocol(msg); // 串口回调数据

	return (result);
}

void set_modbus_slave_address(int add)
{
	modbus_set_slave(mctx, add);
}
