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
#include <libopencm3/stm32/timer.h>
#include <libopencm3/usb/usbd.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int g_tim_state;
extern gpio_state_struct g_gpio_state_list[];
extern usbd_device* usbd_dev;
extern uint8_t g_usb_report_buf[HID_REPORT_SIZE];
extern bool g_usb_initialized;

/* Private function prototypes -----------------------------------------------*/
void tim2_isr_callback (void);
extern void keyboard_generate_report (void);

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

  /* Set timer prescaler. 72MHz/72 => 1000,000 counts per second. */
  TIM_PSC (TIM2) = 72;

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
  tim2_isr_callback();
}

uint32_t g_key_buf[COLUMN_SIZE] = {0};
void tim2_isr_callback (void)
{

  // TODO: make the global and static

  // bool keys_drive_line (gpio_state_struct l[], int rows[], int ROW_SIZE, int row_index)
  int column_index = g_tim_state / 2;
  if (g_tim_state % 2 == 0 && (column_index < COLUMN_SIZE))
  {
    keys_drive_line (g_gpio_state_list, columns, COLUMN_SIZE, g_tim_state >> 1);
  }
  else if (column_index < COLUMN_SIZE)
  {
    uint32_t column_value = 0;
    for (int i = 0; i < ROW_SIZE; i++)
    {
      column_value       = column_value << 1;
      unsigned int delta = bsp_gpio_pin_read (g_gpio_state_list, rows[i]) ? 0 : 1;
      column_value |= delta;
    }
    g_key_buf[column_index] = column_value;
    printf ("column[%d] = 0x%lX\n", column_index, column_value);
  }
  else if (g_tim_state == 2 * COLUMN_SIZE)
  {
    keyboard_generate_report();
  }
  else if (g_tim_state == 2 * COLUMN_SIZE + 1)
  {
    if (g_usb_initialized)
    {
      while (usbd_ep_write_packet (usbd_dev, 0x81, g_usb_report_buf, sizeof (g_usb_report_buf) / sizeof (uint8_t)) == 0)
        ;
    }
  }
  g_tim_state++;
  if (g_tim_state > 2 * COLUMN_SIZE + 2)
  {
    g_tim_state = 0;
  }
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
