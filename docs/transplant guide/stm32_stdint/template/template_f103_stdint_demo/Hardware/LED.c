#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED_ON(uint8_t ledIndex)
{
    switch(ledIndex)
    {
        case 1:
            GPIO_ResetBits(GPIOA, GPIO_Pin_6);
            break;
        case 2:
            GPIO_ResetBits(GPIOA, GPIO_Pin_7);
            break;
        case 3:
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);
            break;
    };
}

void LED_OFF(uint8_t ledIndex)
{
    switch(ledIndex)
    {
        case 1:
            GPIO_SetBits(GPIOA, GPIO_Pin_6);
            break;
        case 2:
            GPIO_SetBits(GPIOA, GPIO_Pin_7);
            break;
        case 3:
            GPIO_SetBits(GPIOC, GPIO_Pin_13);
            break;
    };
}


void LED_Turn(uint8_t ledIndex)
{
    switch(ledIndex)
    {
        case 1:
            if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_6) == 0)
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_6);
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_6);
            }            
            break;
        case 2:
            if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7) == 0)
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_7);
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_7);
            }
            break;
        case 3:
            if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)
            {
                GPIO_SetBits(GPIOC, GPIO_Pin_13);
            }
            else
            {
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);
            }
            break;
    };
}
