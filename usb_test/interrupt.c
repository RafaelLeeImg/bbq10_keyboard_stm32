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
unsigned int g_tim_state;
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

  /* Set timer prescaler. 72MHz/7200 => 10,000 counts per second. */
  TIM_PSC (TIM2) = 7200;

  /* End timer value. If this is reached an interrupt is generated. */
  TIM_ARR (TIM2) = 1000;

  /* Update interrupt enable. */
  TIM_DIER (TIM2) |= TIM_DIER_UIE;

  /* Start timer. */
  TIM_CR1 (TIM2) |= TIM_CR1_CEN;
}

void tim2_isr (void)
{
  TIM_SR (TIM2) &= ~TIM_SR_UIF; /* Clear interrrupt flag. */

  // bsp_gpio_toggle (LED0_PORT, LED0_PIN);
  bsp_gpio_toggle (g_gpio_state_list, LED0);
  // printf ("TIM2 irq\n");

  // TODO: make the global and static
  GPIO_LIST rows[]         = {KEY_ROW_1, KEY_ROW_2, KEY_ROW_3, KEY_ROW_4, KEY_ROW_5, KEY_ROW_6, KEY_ROW_7};
  int row_size             = sizeof (rows) / sizeof (GPIO_LIST);
  GPIO_LIST columns[]      = {KEY_COL_1, KEY_COL_2, KEY_COL_3, KEY_COL_4, KEY_COL_5};
  unsigned int column_size = sizeof (columns) / sizeof (GPIO_LIST);

  // bool keys_drive_line (gpio_state_struct l[], int rows[], int row_size, int row_index)
  if (g_tim_state % 2 == 0 && ((g_tim_state / 2) < column_size))
  {
    keys_drive_line (g_gpio_state_list, columns, column_size, g_tim_state >> 1);
  }
  else if ((g_tim_state / 2) < column_size)
  {
    uint32_t column_value = 0;
    for (int i = 0; i < row_size; i++)
    {
      column_value       = column_value << 1;
      unsigned int delta = bsp_gpio_pin_read (g_gpio_state_list, rows[i]) ? 0 : 1;
      column_value |= delta;
    }
    printf ("column = %d, column value = 0x%lX\n", (g_tim_state / 2), column_value);
  }

  g_tim_state++;
  if (g_tim_state > (2 * (unsigned int)column_size + 1))
  {
    g_tim_state = 0;
  }
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
