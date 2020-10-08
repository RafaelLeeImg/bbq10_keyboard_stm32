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

  // , 1 for GPIOx_ODR pull-up, Table 20. Port bit configuration table
  {GPIOA, GPIO8, 1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_COL_1
  {GPIOB, GPIO11, 1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},    // KEY_COL_2
  {GPIOA, GPIO15, 1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},    // KEY_COL_3
  {GPIOB, GPIO8, 1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_COL_4
  {GPIOA, GPIO0, 1, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_COL_5

  {GPIOA, GPIO2, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // LED0
  {GPIOA, GPIO6, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // BACK_LIGHT0
  {0, 0, 0, 0, 0},                                                        // empty
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
void bsp_gpio_clear (gpio_state_struct l[], GPIO_LIST g)
{
  gpio_clear ((l[g]).gpioport, (l[g]).gpios);
}

uint16_t bsp_gpio_port_read (gpio_state_struct l[], GPIO_LIST g)
{
  return gpio_port_read ((l[g]).gpioport);
}

bool bsp_gpio_pin_read (gpio_state_struct l[], GPIO_LIST g)
{
  uint16_t ret = bsp_gpio_port_read (l, g);
  if (ret & (1 << (l[g]).gpios))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool keys_drive_row (gpio_state_struct l[], GPIO_LIST rows[], int row_size, int row_index)
{
  int row = rows[row_index];
  for (int i = 0; i < row_size; i++)
  {
    if (row_index == i)
    {
      bsp_gpio_clear (l, rows[i]);    // ???????????
    }
    else
    {
      bsp_gpio_set (l, rows[i]);    // ???????????
    }
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
 *   Drive row before read.
 */
//  TODO: inline
bool read_key (gpio_state_struct l[], GPIO_LIST columns[], int column_size, GPIO_LIST rows[], int row_size, int index)
{
  // for (int i=0;i<column_size;i++)
  // {
  //   for (int j=0;j<row_size;j++)
  //   {
  //   }
  // }
  int column = index % row_size;
  int row    = index / row_size;

  // for (int i = 0; i < row_size; i++)
  // {
  //   uint32_t row_value = 0;
  //   // drive row
  //   for (int j = 0; j < column_size; j++)
  //   {
  //     row_value = row_value << 1;
  //     row_value &= bsp_gpio_pin_read (l, columns[j]) ? 1 : 0;
  //   }
  // }

  return bsp_gpio_pin_read (l, columns[column]);
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
