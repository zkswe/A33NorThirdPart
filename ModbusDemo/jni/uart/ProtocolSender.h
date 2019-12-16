/*
 * ProtocolSender.h
 *
 *  Created on: Sep 8, 2017
 *      Author: guoxs
 */

#ifndef _UART_PROTOCOL_SENDER_H_
#define _UART_PROTOCOL_SENDER_H_

#include "CommDef.h"
#include "ProtocolData.h"

/** 请求读取从base_register_addr 到 end_register_addr 的寄存器 */
bool readRegisters(int base_register_addr, int end_register_addr);
/** 写单个寄存器*/
bool writeRegist(UINT16 reg, const UINT16 value);


void uartMessageSetNull(void);
int uartMessageEmpty(void);
SProtocolData *uartMessageGetFront(void);
int uartMessageInQueue(SProtocolData data);
SProtocolData *uartMessageDeQueue(void);
#endif /* _UART_PROTOCOL_SENDER_H_ */
