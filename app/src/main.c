/* main.c */

#include <stdio.h>
#include <stm32f1xx.h>
#include <delay.h>
#include <stddef.h>  // for size_t
#include <unistd.h>  // for STDERR_FILENO
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "ztos.h"

void task1Rtn (void);
void task2Rtn (void);

int main(void)
{

	/* Init GPIO Port C, Init GPIO Port A , Init GPIO Port A, Init AFIO */
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	/* Set GPIO on PORT C - P13 */
	GPIOC->CRH &= ~(0xF << ((13 - 8) * 4));
	GPIOC->CRH |=  (0x2 << ((13 - 8) * 4));

	if (usartDrvInit (USART_DRV_1, 115200) != OK)
		while (1){/* system halt */}
		
	if (usartDrvEnable (USART_DRV_1) != OK)
		while (1){/* system halt */}

	SYSTEM_LOG ("USART Drv 1 Enabled!");

	if (zSchedInit() != OK)
	{
	SYSTEM_LOG ("Error on zSchedInit");	
	}

	if (zTaskCreate ("task1", 512, task1Rtn) != OK)
	{
		SYSTEM_LOG ("Error on zTaskCreate - task1");	
	}

	if (zTaskCreate ("task2", 512, task2Rtn) != OK)
	{
		SYSTEM_LOG ("Error on zTaskCreate - task1");	
	}

	// while(1)
	// {
		/* Blink */
		// SYSTEM_LOG ("System heartbeat");

		/* DBG: Toggle LED*/
		// GPIOC->ODR ^= 1 << 13;

	// 	zTaskDelay (TICKS_PER_SEC);
	// }
}


void task1Rtn (void)
{
	while (1)
	{
		SYSTEM_LOG ("Hello on task 1");
		/* DBG: Toggle LED*/
		GPIOC->ODR ^= 1 << 13;
		zTaskDelay (TICKS_PER_SEC);
	}
	
}

void task2Rtn (void)
{
	while (1)
	{
		SYSTEM_LOG ("Hello on task 2");
		/* DBG: Toggle LED*/
		zTaskDelay (TICKS_PER_SEC * 2);
	}
	
}