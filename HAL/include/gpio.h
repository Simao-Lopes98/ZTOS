/* gpio.h */

/*
See RM0008_ref section 9 for details
*/

#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stm32f1xx.h>
#include "gpio.h"

typedef enum
{
    GPIO_ERROR = -1,
    GPIO_OK
} GPIO_STATUS;

typedef enum
{
    GPIO_A = 0,
    GPIO_B,
    GPIO_C,
    GPIO_D,
    GPIO_E 
} gpio_inst_t;

/* GPIO driver struct */
typedef struct
{
    __IO uint32_t CRL;  /* Port configuration register low */
    __IO uint32_t CRH;  /* Port configuration register high */
    __IO uint32_t IDR;  /* Port input data register */
    __IO uint32_t ODR;  /* Port output data register */
    __IO uint32_t BSRR; /* Port bit set/reset register */
    __IO uint32_t BRR;  /* Port bit reset register */
    __IO uint32_t LCKR; /* Port configuration lock register */
}gpio_t;


/* forward decl */

extern gpio_t * gpio_drv_init (gpio_inst_t gpio_inst);


#endif /* GPIO_H */