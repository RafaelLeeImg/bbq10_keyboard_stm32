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
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/f1/rcc.h>    // RCC_TIM7
#include <libopencm3/stm32/timer.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
void tim7_interrupt_setup (void)
{
  rcc_periph_clock_enable (RCC_TIM7);

  /* Without this the timer interrupt routine will never be called. */
  nvic_enable_irq (NVIC_TIM7_IRQ);
  nvic_set_priority (NVIC_TIM7_IRQ, 1);

  /* Set timer start value. */
  TIM_CNT (TIM7) = 1;

  /* Set timer prescaler. 72MHz/1440 => 50000 counts per second. */
  TIM_PSC (TIM7) = 1440;

  /* End timer value. If this is reached an interrupt is generated. */
  TIM_ARR (TIM7) = 50000;

  /* Update interrupt enable. */
  TIM_DIER (TIM7) |= TIM_DIER_UIE;

  /* Start timer. */
  TIM_CR1 (TIM7) |= TIM_CR1_CEN;
}

void tim7_isr (void)
{
  printf ("tim7 irq\n");
  TIM_SR (TIM7) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */
}
/************************ (C) COPYRIGHT ************************END OF FILE****/
