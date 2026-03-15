/*
 * ztos.c
 * ZTOS Real-Time Operating System Implementation
 */

/* Includes */
#include "ztos.h"

/* Private variables */
static sllCtrl_t * taskList = NULL;

/* Private function prototypes */
static void zIdleTask (void);
static STATUS createIdleTask (void);
static void setupTaskStack (zTask_t * task);

/* Public functions */
STATUS zSchedInit (void)
{
    /* Start Timer for tick */
    (void) timerDrvInit(TIMER_DRV_2);
    
    /* Create SLL for task nodes */
    taskList = sllCreate ();
    if (taskList != OK)
    {
        return ERROR;
    }
    
    /* Create Idle Task */
    if (createIdleTask() != OK)
    {
        return ERROR;
    }

	(void) timerDrvEnable (TIMER_DRV_2);

    return OK;
}

STATUS zTaskCreate (char * name,
                    uint32_t stackSize,
                    zTaskHandler taskEntryFn)
{
    zTask_t * tasktoCreate = NULL;

    if (name == NULL)
    {
        return ERROR;
    }

    if (taskEntryFn == NULL)
    {
        return ERROR;
    }

    tasktoCreate = (zTask_t *) calloc (1, sizeof(zTask_t));
    if (tasktoCreate == NULL)
    {
        return ERROR;
    }

    /* Allocate stack size */
    tasktoCreate->stackPtr = calloc (stackSize, sizeof (uint32_t));
    if (tasktoCreate->stackPtr == NULL)
    {
        return ERROR;
    }

    tasktoCreate->stackSize = stackSize;
    (void) strncpy (tasktoCreate->name, name, MAX_TASK_NAME);
    tasktoCreate->entryFn = taskEntryFn;
    tasktoCreate->status = TASK_RUNNING;

    /* Fabricate entry point */
    setupTaskStack (tasktoCreate);

    if (sllInsertBack(taskList, (void *) tasktoCreate) != OK)
    {
        return ERROR;
    }
    
    return OK;
}

void TIM2_IRQHandler(void)
{
    /* Clear */
    TIM2->SR &= ~TIM_SR_UIF;

    /* DBG: Toggle LED*/
    GPIOC->ODR ^= 1 << 13;
}

/* Private functions */

/*
TODO: The tasks should not return and have no input args. 
This could be improved.. To do so, LR and R0 have to be properly setup 
*/
static void setupTaskStack (zTask_t * task)
{
    uint32_t * stack = (uint32_t *) ((uint32_t *) task->stackPtr + task->stackSize);

    /* Align memory to 8 Bits */
    stack = (uint32_t *)(((uint32_t)stack) & ~0x07);

    /* Setup the PSR register with T bit set */
    * (-- stack) = 0x01000000;

    /* Setup the PC register to the function entry point */
    * (-- stack) = (uint32_t * ) task->entryFn;

    /* Setup the LR - Tasks should not return */
    * (-- stack) = (uint32_t * ) 0xDEADBEEF;

    for (size_t i = 0; i < DUMMY_REGISTERS_NUM; i++)
    {
        /* Setup the GP Register with a dummy value */
        * (-- stack) = 0xDEADBEEF;
    }
    
    /* Save current stack pointer */
    task->currentStackPtr = stack;
}

static void zIdleTask (void)
{
    while (1)
    {
        __asm volatile ("nop");
    }
    
}

static STATUS createIdleTask (void)
{
    zTask_t * idleTcb = NULL;

    idleTcb = (zTask_t *) calloc (1, sizeof(zTask_t));
    if (idleTcb == NULL)
    {
        return ERROR;
    }

    /* Allocate space for idle task stack */
    idleTcb->stackPtr = (void *) calloc (   IDLE_TASK_STACK_SIZE, 
                                            sizeof (uint32_t));
    if (idleTcb->stackPtr == NULL)
    {
        return ERROR;
    }

    idleTcb->stackSize = IDLE_TASK_STACK_SIZE;
    (void) strncpy (idleTcb->name, IDLE_TASK_NAME, MAX_TASK_NAME);
    idleTcb->entryFn = zIdleTask;
    idleTcb->status = TASK_RUNNING; /* Idle task has to be ALWAYS running */

    /* Add idle task to the list */
    if (sllInsertBack (taskList, (void *) idleTcb) != OK)
    {
        return ERROR;
    }

    return OK;
}