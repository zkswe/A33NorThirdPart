/*
 * ProtocolSender.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: guoxs
 */

#include "uart/ProtocolSender.h"
#include "uart/UartContext.h"
#include <stdio.h>

#include "utils/Log.h"

static Mutex sLock;
UartMessageSequeue MessageSequeue = {0};


/*  cmdID = modbus命令，目前仅支持03,06命令
	reg   = modbus寄存器
	*pData = 数据指针，读操作，赋值NULL
	len    = 寄存器个数

           返回值：true 操作成功，false 操作失败，队列已满
*/

//消息队列置空
void uartMessageSetNull(void)
{
	MessageSequeue.front = 0;
	MessageSequeue.rear  = 0;
}

//判断队列是否为空
int uartMessageEmpty(void)
{
	if(MessageSequeue.rear == MessageSequeue.front)
		return (1);
	else
		return (0);
}

//取队列头结点，不删除头结点
SProtocolData *uartMessageGetFront(void)
{
	if(uartMessageEmpty())
		return ((SProtocolData *)0);
	else
		return (&(MessageSequeue.uartMessage[(MessageSequeue.front + 1) % UART_MESSAGE_NUM_MAX]));
}

//入队
int uartMessageInQueue(SProtocolData data)
{
	Mutex::Autolock _l(sLock);
	if((MessageSequeue.rear+1) % UART_MESSAGE_NUM_MAX == MessageSequeue.front)
		return (0);
	else
	{
		MessageSequeue.rear = (MessageSequeue.rear+1)	% UART_MESSAGE_NUM_MAX;
		MessageSequeue.uartMessage[MessageSequeue.rear] = data;
		return (1);
	}
}

//出队
SProtocolData *uartMessageDeQueue(void)
{
	Mutex::Autolock _l(sLock);
	if(uartMessageEmpty())
		return ((SProtocolData *)0);
	else
	{
		MessageSequeue.front = (MessageSequeue.front+1) % UART_MESSAGE_NUM_MAX;
		return (&(MessageSequeue.uartMessage[MessageSequeue.front]));
	}
}

bool readRegisters(int base_register_addr, int end_register_addr) {
    LOGD(" readRegisters %d ~ %d", base_register_addr, end_register_addr);
    SProtocolData uartMessageTemp = {0};
    uartMessageTemp.err = 0;
    uartMessageTemp.cmd = TYPE_CMD_REG_READ;
    uartMessageTemp.reg = base_register_addr;
    uartMessageTemp.nb  = end_register_addr - base_register_addr + 1;

    if (uartMessageTemp.nb > 128) {
        LOGE("too much register, %d", uartMessageTemp.nb);
        return false;
    }
    int result = uartMessageInQueue(uartMessageTemp);//入队
    //LOGD("uartMessageInQueue result = %d;   front = %d; rear = %d;\n  ", result, MessageSequeue.front, MessageSequeue.rear);

    if(result == 0)
        return (false);
    else
        return (true);
}

bool writeRegist(UINT16 reg, const UINT16 value) {
  SProtocolData uartMessageTemp = {0};

    uartMessageTemp.err = 0;
    uartMessageTemp.cmd = TYPE_CMD_REG_WRITE;
    uartMessageTemp.reg = reg;
    uartMessageTemp.nb  = 1;

    uartMessageTemp.data[0] = value;

    int result = uartMessageInQueue(uartMessageTemp);//入队

    if(result == 0)
        return (false);
    else
        return (true);
}
