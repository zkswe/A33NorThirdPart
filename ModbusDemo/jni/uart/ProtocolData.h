/*
 * ProtocolData.h
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */

#ifndef _UART_PROTOCOL_DATA_H_
#define _UART_PROTOCOL_DATA_H_

#include <string>
#include "CommDef.h"

#define UART_MESSAGE_NUM_MAX  64   //消息最大数量64个

typedef struct {
	BYTE   cmd;   //modbus命令
	UINT16 reg;   //modbus寄存器
	BYTE   nb;    //modbus数据个数
	int err;      //错误信息
	UINT16 data[128]; //modbus数据指针
} SProtocolData;

typedef struct
{
	SProtocolData uartMessage[UART_MESSAGE_NUM_MAX];
	int front; //队头
	int rear;  //队尾
}UartMessageSequeue;

enum
{
	TYPE_CMD_REG_READ   = 0x03,  //读保持寄存器
	TYPE_CMD_REG_WRITE  = 0x06,  //写单个寄存器
	TYPE_CMD_REG_WRITES = 0x10,  //写多个寄存器
};

#endif /* _UART_PROTOCOL_DATA_H_ */
