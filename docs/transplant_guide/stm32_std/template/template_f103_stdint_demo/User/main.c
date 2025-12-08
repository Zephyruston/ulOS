#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "Key.h"
#include "Timer.h"
#include "ulOS_thread.h"

void thread0_entry(void *p)
{
    while(1)
    {
        LED_Turn(1);
        ul_thread_delay(100);
    }
}

void thread1_entry(void *p)
{
    while(1)
    {
        LED_Turn(2);
        ul_thread_delay(500);
    }
}

void thread2_entry(void *p)
{
    while(1)
    {
        LED_Turn(3);
        ul_thread_delay(1000);
    }
}

void start_thread_entry(void *p)
{
    ul_thread_t *tid0 = ul_thread_create("t0", thread0_entry, UL_NULL, 512, 0, 1);
    ul_thread_t *tid1 = ul_thread_create("t1", thread1_entry, UL_NULL, 512, 0, 1);
    ul_thread_t *tid2 = ul_thread_create("t2", thread2_entry, UL_NULL, 512, 0, 1);
    
    ul_thread_startup(tid0);
    ul_thread_startup(tid1);
    ul_thread_startup(tid2);
    
    while(1)
    {
        ul_thread_delay(10);
    }
}



int main(void)
{
    /*模块初始化*/
    LED_Init();
    Timer_Init();
    ul_kernel_init();
    
    ul_thread_startup(ul_thread_create("start",start_thread_entry,UL_NULL,512,1,1));
    
    ul_scheduler_start();
    while (1)
    {
        
    }

}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
        ul_tick_increase();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
