/*
 * uart.c
 * UART Hardware Abstraction Layer Implementation
 */

/* Includes */
#include "uart.h"

/* Private variables */

/* USART driver list */
static USART_TypeDef * usartDrvs [4] = 
                    {
                    NULL,
                    USART1,
                    USART2,
                    USART3
                    };

/* Private function prototypes */
static void usart1Init (void);
static STATUS configUsartBaud (USART_TypeDef * usart, uint32_t baud);
static STATUS checkUsartDrvNum (usartDrvNum_t usartNum);

/* Public functions */

/* 
Initializes USART driver using [usartNum] to define the port and [baud]
to define the baudrate
*/
STATUS usartDrvInit (usartDrvNum_t usartNum, uint32_t baud)
{
    if (checkUsartDrvNum(usartNum) != OK)
        return ERROR;

    USART_TypeDef * usartDrv = usartDrvs[usartNum];

    /* Not pretty, it is what it is */
    switch (usartNum)
    {
    case USART_DRV_1:
        usart1Init();
        break;
    case USART_DRV_2:
        /* TODO */
        return ERROR;
        break;
    case USART_DRV_3:
        /* TODO */
        return ERROR;
        break;
    
    default:
        /* Should not reach this */
        return ERROR;
        break;
    }

    if (configUsartBaud (usartDrv, baud) != OK)
    {
        return ERROR;
    }

    /* enable TX and RX */
    usartDrv->CR1 = USART_CR1_TE | USART_CR1_RE;  

    return OK;
}

/* 
Enables the USART driver using [usartNum] to define the port which to enable
*/
STATUS usartDrvEnable (usartDrvNum_t usartNum)
{
    if (checkUsartDrvNum(usartNum) != OK)
        return ERROR;

    USART_TypeDef * usartDrv = usartDrvs[usartNum];
    
    /* enable USART */
    usartDrv->CR1 |= USART_CR1_UE;
    
    return OK;
}

/* 
Stops the USART driver using [usartNum] to define the port which to stop
*/
STATUS usartDrvStop (usartDrvNum_t usartNum)
{
    if (checkUsartDrvNum(usartNum) != OK)
        return ERROR;

    USART_TypeDef * usartDrv = usartDrvs[usartNum];
    
    /* stop USART */
    usartDrv->CR1 &= ~USART_CR1_UE;
    
    return OK;
}

/* 
Sets the DR register to buf contents 
*/
void usartWrite (usartDrvNum_t usartNum, char * buf, size_t len)
{
    USART_TypeDef * usartDrv = NULL;

    if (buf == NULL)
    {
        return;
    }

    if (checkUsartDrvNum (usartNum) != OK)
    {
        return;
    }

    usartDrv = usartDrvs[usartNum];

    /* USART stopped or not enabled  */
    if ((usartDrv->CR1 & USART_CR1_UE) == 0)
    {
        return;
    }

	while ((usartDrv->SR & USART_SR_TXE) == 0)
	{ /* Wait for TX buffer to be ready */	}
	
	/* Set chars on DR buf */
	for (size_t i = 0; i < len; i++)
	{
		usartDrv->DR = buf[i];
		/* Wait until transmit is done */
		while (!(usartDrv->SR & USART_SR_TC));
	}
}

/*
Sets buf to the contents of the DR register
*/
size_t usartRead (usartDrvNum_t usartNum, char* buf, size_t len)
{
    USART_TypeDef * usartDrv = NULL;
    size_t count = 0;

    if (buf == NULL)
    {
        return 0;
    }

    if (checkUsartDrvNum (usartNum) != OK)
    {
        return 0;
    }

    usartDrv = usartDrvs[usartNum];

    /* USART stopped or not enabled  */
    if ((usartDrv->CR1 & USART_CR1_UE) == 0)
    {
        return 0;
    }

    while ((usartDrv->SR & USART_SR_RXNE) == 0)
    { /* waiting for data */}

    /* Got a char. Clearing RXNE */
    usartDrv->SR &= ~USART_SR_RXNE;

    while ( usartDrv->DR != '\r'&& 
            usartDrv->DR != '\n'&& 
            count < len)
    {
        buf [count] = usartDrv->DR;
        count++;
        
        while ((usartDrv->SR & USART_SR_RXNE) == 0)
        { /* waiting for data */}

        /* Got a char. Clearing RXNE */
        usartDrv->SR &= ~USART_SR_RXNE;
    }
    
    
    return count;
}

/* Stub _write. Function called by printf - Locked to USART1 */
int _write(int fd, char *buf, size_t len)
{
	usartWrite (USART_DRV_1, buf, len);
	return len;
}

/* Private functions */

/* Init USART_1 */
static void usart1Init (void)
{
    /* Init USART 1, Alternate function and IO Port A */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | 
                    RCC_APB2ENR_AFIOEN |
                    RCC_APB2ENR_IOPAEN;

	/* 
	Configure as Alternate function output Push-pull 
	and Output mode, max speed
	*/

	/* Clear PA9 and P10 config */
	GPIOA->CRH &= ~GPIO_CRH_CNF9_Msk | GPIO_CRH_CNF10_Msk;
	GPIOA->CRH &= ~GPIO_CRH_MODE9_Msk| GPIO_CRH_CNF10_Msk;

	/* MODE9 = 11 (50 MHz), CNF9 = 10 (Alternate Function push-pull) */
	GPIOA->CRH |=  GPIO_CRH_CNF9_1;
	GPIOA->CRH |=  GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0;

    /* MODE10 = 0, CNF10 = 1 (Input floating)*/
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
}

/* Check USART number input by user */
static STATUS checkUsartDrvNum (usartDrvNum_t usartNum)
{
    if (usartNum > USART_DRV_3 || usartNum < USART_DRV_1)
    {
        return ERROR;
    }

    return OK;
}

/* Configures the baud rate - See section 27.3.4 from RM0008 Rev 21 */
static STATUS configUsartBaud (USART_TypeDef * usart, uint32_t baud)
{
	/* 
    Example for a baud of 115200 
	Baud = fCLK / (16 * USARTDIV) 
	USARTDIV = fCLK / (16 * Baud)
	USARTDIV = 8,000,000 / (16 * 115200)
	USARTDIV = 4.340277 (...)
	Mantissa = 4
	Div = 0.340277 * 16 = 5.44
	*/

    /* TODO: Add floating point cal */
    // int e = 0;
    // uint8_t m = 0;
    // double baudDiv = CLK_RATE / (16 * baud);
    // m = (uint8_t)frexp(baudDiv, &e);

    usart->BRR = (4 << 4) | (5 << 0);

    /* TODO: Add error calculation */
    return OK;
}