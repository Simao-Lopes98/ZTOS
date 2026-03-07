/* TIMER Header File */

/* 
 * File: timer.h
 * Purpose: Timer hardware abstraction layer
 */

#ifndef TIMER_H
#define TIMER_H

/* Includes */
#include <stdio.h>
#include <stm32f1xx.h>
#include "clock.h"
#include "app.h"

/* Macros */

/* Type Definitions */
typedef enum
{
    TIMER_DRV_1,
    TIMER_DRV_2,
    TIMER_DRV_3,
    TIMER_DRV_4
} timerDrvNum_t;

/* Function Declarations */
extern STATUS timerDrvInit (timerDrvNum_t timerNum);
extern STATUS timerDrvEnable (timerDrvNum_t timerNum);
extern STATUS timerDrvStop (timerDrvNum_t timerNum);
#endif /* TIMER_H */