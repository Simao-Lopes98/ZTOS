/* gpio.c */

/*
GPIO HAL
*/

#include "gpio.h"

static uint32_t * const gpio_instance [] =
{
    (uint32_t *) GPIOA_BASE,
    (uint32_t *) GPIOB_BASE,
    (uint32_t *) GPIOC_BASE,
    (uint32_t *) GPIOD_BASE,
    (uint32_t *) GPIOE_BASE
};

gpio_t * gpio_drv_init (gpio_inst_t gpio_inst)
{
    /* Turn the IO clk if not */
    if ( (RCC->APB2ENR & RCC_APB2ENR_IOPCEN) == 0)
    {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    }

    return (gpio_t *) gpio_instance[gpio_inst];
}
