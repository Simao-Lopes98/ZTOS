/* UART Header File */

/* 
 * File: uart.h
 * Purpose: UART hardware abstraction layer
 */


#ifndef UART_H
#define UART_H

/* Includes */
#include <stdio.h>
#include <string.h>
#include <stm32f1xx.h>
#include "clock.h"
#include "app.h"
#include <math.h>

/* Macros */
#define USART_RCV_BUFF_MAX_SIZE 64

/* Type Definitions */
typedef enum
{
    USART_DRV_1 = 1,
    USART_DRV_2,
    USART_DRV_3,
} usartDrvNum_t;

/* Function Declarations */

extern STATUS usartDrvInit (usartDrvNum_t usartNum, uint32_t baud);
extern STATUS usartDrvEnable (usartDrvNum_t usartNum);
extern STATUS usartDrvStop (usartDrvNum_t usartNum);
extern void usartWrite (usartDrvNum_t usartNum, char * buf, size_t len);
extern size_t usartRead (usartDrvNum_t usartNum, char* buf, size_t len);
extern int _write(int fd, char *buf, size_t len);

#endif /* UART_H */