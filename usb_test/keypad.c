/**
 ******************************************************************************
 * @file
 * @author  Rafael Lee
 * @version v0.0.1
 * @date
 * @brief   keypad.c module
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "keypad.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// TODO:rename to gpio_table
gpio_state_struct g_gpio_state_list[] = {
  // gpioport gpios reset_state mode config
  {GPIOB, GPIO12, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // KEY_ROW_1
  {GPIOB, GPIO10, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // KEY_ROW_2
  {GPIOB, GPIO9, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_ROW_3
  {GPIOA, GPIO1, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_ROW_4
  {GPIOA, GPIO3, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_ROW_5
  {GPIOA, GPIO4, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_ROW_6
  {GPIOA, GPIO5, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_ROW_7

  {GPIOA, GPIO8, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_COL_1
  {GPIOB, GPIO11, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // KEY_COL_2
  {GPIOA, GPIO15, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // KEY_COL_3
  {GPIOB, GPIO8, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_COL_4
  {GPIOA, GPIO0, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_COL_5
  {GPIOA, GPIO2, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // LED0
  {GPIOA, GPIO6, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // BACK_LIGHT0
  {0, 0, 0, 0, 0},                                                         // empty
};

/* Private function prototypes -----------------------------------------------*/
int my_test (int a, int b);
/* Private functions ---------------------------------------------------------*/

/* ---------------------------------------------------------------------------*/
/**
 * @brief
 * @param  None
 * @retval None
 */
/* ---------------------------------------------------------------------------*/
int my_test (int a, int b)
{
  return a + b;
}

/**
 * \par Function
 *   function
 * \par Description
 *   desc
 * \par Output
 *   None
 * \par return
 *   ret
 * \par Others
 *
 */
void bsp_gpio_init (gpio_state_struct l[])
{
  //   gpio_state_struct* l = gpio_list;
  while (0 != l->gpioport)
  {
    gpio_set_mode (l->gpioport, l->mode, l->config, l->gpios);
    l++;
  }
}

/**
 * \par Function
 *   function
 * \par Description
 *   desc
 * \par Output
 *   None
 * \par return
 *   ret
 * \par Others
 *
 */
void bsp_gpio_toggle (gpio_state_struct l[], GPIO_LIST g)
{
  gpio_toggle ((l[g]).gpioport, (l[g]).gpios);
}

/**
 * \par Function
 *   function
 * \par Description
 *   desc
 * \par Output
 *   None
 * \par return
 *   ret
 * \par Others
 *
 */
void bsp_gpio_set (gpio_state_struct l[], GPIO_LIST g)
{
  gpio_set ((l[g]).gpioport, (l[g]).gpios);
}

/**
 * \par Function
 *   function
 * \par Description
 *   desc
 * \par Output
 *   None
 * \par return
 *   ret
 * \par Others
 *
 */
void bsp_gpio_reset (gpio_state_struct l[], GPIO_LIST g)
{
  gpio_clear ((l[g]).gpioport, (l[g]).gpios);
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
