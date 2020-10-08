/**
 ******************************************************************************
 * @file
 * @author
 * @version v0.0.1
 * @date
 * @brief   xxxx.c module
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "interrupt.h"
#include "keypad.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/f1/rcc.h>    // RCC_TIM7
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern gpio_state_struct g_gpio_state_list[];
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* ---------------------------------------------------------------------------*/
/**
 * @brief
 * @param  None
 * @retval None
 */
/* ---------------------------------------------------------------------------*/

/**
 * \par Function
 *   function
 * \par Description
 *   description
 * \par Output
 *   None
 * \par return
 *   ret
 * \par Others
 *
 */

void tim2_interrupt_setup (void)
{
  rcc_periph_clock_enable (RCC_TIM2);

  /* Without this the timer interrupt routine will never be called. */
  nvic_enable_irq (NVIC_TIM2_IRQ);
  nvic_set_priority (NVIC_TIM2_IRQ, 1);

  /* Set timer start value. */
  TIM_CNT (TIM2) = 1;

  /* Set timer prescaler. 72MHz/1440 => 50000 counts per second. */
  TIM_PSC (TIM2) = 1440;

  /* End timer value. If this is reached an interrupt is generated. */
  TIM_ARR (TIM2) = 50000;

  /* Update interrupt enable. */
  TIM_DIER (TIM2) |= TIM_DIER_UIE;

  /* Start timer. */
  TIM_CR1 (TIM2) |= TIM_CR1_CEN;
}

void tim2_isr (void)
{
  // bsp_gpio_toggle (LED0_PORT, LED0_PIN);
  bsp_gpio_toggle (g_gpio_state_list, LED0);
  printf ("TIM2 irq\n");
  TIM_SR (TIM2) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
