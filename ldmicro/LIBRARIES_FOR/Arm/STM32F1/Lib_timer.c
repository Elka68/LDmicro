/**
 * Copyright (C) JG 2019
 */

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "interrupts.h"

#include "lib_timer.h"

/**
  * @brief  Configure Timer behaviour
  * @brief  TIM2 to TIM4 are 16 bits timers
  * @param  TIMx = TIM2 to TIM4
  * @param  prediv = timer prescaler
  * @param  period = timer periodicity
  * @retval None
  */
void LibTimer_Init(TIM_TypeDef *TIMx, uint16_t prediv, uint32_t period)
{
    TIM_TimeBaseInitTypeDef timerInitStructure; // parametrage Timer

    if(TIMx == TIM2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // activation clock
    if(TIMx == TIM3)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // activation clock
    if(TIMx == TIM4)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // activation clock

    timerInitStructure.TIM_Prescaler = prediv;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = period;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0; // tourne indefiniment
    TIM_TimeBaseInit(TIMx, &timerInitStructure);

    TIM_Cmd(TIMx, ENABLE);                     // activation Timer
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); // activation interruptions Timer

    Interrupts_Enable(); // autorisation generale des interuptions
}

/**
  * @brief  Configure Timer interrupts
  * @brief  TIM2 to TIM4 are 16 bits timers
  * @param  TIMx = TIM2 to TIM4
  * @param  state = ENABLE or DISABLE
  * @retval None
  */
void LibTimer_Interrupts(TIM_TypeDef *TIMx, FunctionalState state)
{
    NVIC_InitTypeDef nvicStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // niveaux de priorites

    nvicStructure.NVIC_IRQChannel = 0;
    if(TIMx == TIM2)
        nvicStructure.NVIC_IRQChannel = TIM2_IRQn; // N� d'interruption
    if(TIMx == TIM3)
        nvicStructure.NVIC_IRQChannel = TIM3_IRQn; // N� d'interruption
    if(TIMx == TIM4)
        nvicStructure.NVIC_IRQChannel = TIM4_IRQn; // N� d'interruption

    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0; // priorite 0-1
    nvicStructure.NVIC_IRQChannelSubPriority = 1;        // sous priorite 0-7
    nvicStructure.NVIC_IRQChannelCmd = state;            // ENABLE or DISABLE

    NVIC_Init(&nvicStructure);
}

/**
  * @brief  Stop Timer and interrupts
  * @brief  TIM2 to TIM4 are 16 bits timers
  * @param  TIMx = TIM2 to TIM4
  * @retval None
  */
void LibTimer_Stop(TIM_TypeDef *TIMx)
{
    TIM_ITConfig(TIMx, TIM_IT_Update, DISABLE); // desactivation interruptions Timer

    TIM_Cmd(TIMx, DISABLE); // desactivation Timer
}
