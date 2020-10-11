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

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ from ultimate hacking matrix
#define KEYBOARD_CONFIGURATION_COUNT (1)
#define KEYBOARD_LAYER_NUMBER        (2)

#define KEYBOARD_COLUMN_NUMBER COLUMN_SIZE
#define KEYBOARD_ROW_NUMBER    ROW_SIZE

// uint16_t g_key_lut[KEYBOARD_CONFIGURATION_COUNT][KEYBOARD_LAYER_NUMBER][KEYBOARD_ROW_NUMBER][KEYBOARD_COLUMN_NUMBER]

// { {{{}}}, };

// uint16_t g_key_lut_tmp[1][2][KEYBOARD_ROW_NUMBER][KEYBOARD_COLUMN_NUMBER] = {
//   // config 0 ******************** QWERT 0  ********************
//   {{{K_Q, K_E, K_R, K_U, K_O},
//     {K_W, K_S, K_G, K_H, K_L},
//     {K_FN_LEFT, K_D, K_T, K_Y, K_I},
//     {K_A, K_P, K_SHIFT_RIGHT, K_ENTER, K_BACKSPACE},
//     {K_ALT_LEFT, K_X, K_V, K_B, K_PRTSC},
//     {K_SPACE, K_Z, K_C, K_N, K_M},
//     {K_BACK_QUOTE, K_SHIFT_LEFT, K_F, K_J, K_K}},

//    // Config 2 QWERT normal *************************************************
//    {{K_Q, K_E, K_R, K_U, K_O},
//     {K_W, K_S, K_G, K_H, K_L},
//     {K_FN_LEFT, K_D, K_T, K_Y, K_I},
//     {K_A, K_P, K_SHIFT_RIGHT, K_ENTER, K_BACKSPACE},
//     {K_ALT_LEFT, K_X, K_V, K_B, K_PRTSC},
//     {K_SPACE, K_Z, K_C, K_N, K_M},
//     {K_BACK_QUOTE, K_SHIFT_LEFT, K_F, K_J, K_K}}}};

uint16_t g_key_lut[1][2][KEYBOARD_COLUMN_NUMBER][KEYBOARD_ROW_NUMBER] = {
  // config 0 ******************** QWERT 0  ********************
  {{
     // SYN and $ are wrong
     {K_BACK_QUOTE, K_SPACE, K_ALT_LEFT, K_A, K_FN_RIGHT, K_W, K_Q},
     {K_SHIFT_LEFT, K_Z, K_X, K_P, K_D, K_S, K_E},
     {K_F, K_C, K_V, K_SHIFT_RIGHT, K_T, K_G, K_R},
     {K_J, K_N, K_B, K_ENTER, K_Y, K_H, K_U},
     {K_K, K_M, K_BACK_QUOTE, K_BACKSPACE, K_I, K_L, K_O},
   },

   // Config 2 QWERT normal *************************************************
   {
     {K_BACK_QUOTE, K_SPACE, K_ALT_LEFT, K_A, K_FN_RIGHT, K_W, K_Q},
     {K_SHIFT_LEFT, K_Z, K_X, K_P, K_D, K_S, K_E},
     {K_F, K_C, K_V, K_SHIFT_RIGHT, K_T, K_G, K_R},
     {K_J, K_N, K_B, K_ENTER, K_Y, K_H, K_U},
     {K_K, K_M, K_BACK_QUOTE, K_BACKSPACE, K_I, K_L, K_O},
   }}};

uint8_t g_usb_report_buf[HID_REPORT_SIZE];
uint8_t g_usb_report_buf_previous[HID_REPORT_SIZE];
// uint32_t g_key_buf[KEYBOARD_COLUMN_NUMBER] = {0};
extern uint32_t g_key_buf[];
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// TODO:rename to gpio_table
gpio_state_struct g_gpio_state_list[] = {
  // gpioport gpios reset_state mode config
  {GPIOB, GPIO12, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},    // KEY_ROW_1
  {GPIOB, GPIO10, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},    // KEY_ROW_2
  {GPIOB, GPIO9, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_ROW_3
  {GPIOA, GPIO1, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_ROW_4
  {GPIOA, GPIO3, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_ROW_5
  {GPIOA, GPIO4, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_ROW_6
  {GPIOA, GPIO5, 0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN},     // KEY_ROW_7

  // , 1 for GPIOx_ODR pull-up, Table 20. Port bit configuration table
  {GPIOA, GPIO8, 1, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_COL_1
  {GPIOB, GPIO11, 1, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // KEY_COL_2
  // TODO, avoid PA15 in next version, since PA15 is JTDI
  {GPIOA, GPIO15, 1, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // KEY_COL_3
  {GPIOB, GPIO8, 1, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_COL_4
  {GPIOA, GPIO0, 1, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},     // KEY_COL_5

  {GPIOA, GPIO2, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // LED0
  {GPIOA, GPIO6, 0, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL},    // BACK_LIGHT0
  {0, 0, 0, 0, 0},                                                        // empty
};

// GPIO_LIST rows[] = {KEY_ROW_1, KEY_ROW_2, KEY_ROW_3, KEY_ROW_4, KEY_ROW_5, KEY_ROW_6, KEY_ROW_7};
// GPIO_LIST columns[] = {KEY_COL_1, KEY_COL_2, KEY_COL_3, KEY_COL_4, KEY_COL_5};

static uint32_t g_fn_mask[COLUMN_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
int my_test (int a, int b);
uint32_t keyboard_is_fn_pressed (void);
int32_t keyboard_pressed_key_count (void);
void keyboard_generate_report (void);
/* Private functions ---------------------------------------------------------*/

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
  // switch ((l[g]).gpioport)
  // {
  // case GPIOA:
  //   printf ("GPIOA, ");
  //   break;
  // case GPIOB:
  //   printf ("GPIOB, ");
  //   break;
  // case GPIOC:
  //   printf ("GPIOC, ");
  //   break;
  // case GPIOD:
  //   printf ("GPIOD, ");
  //   break;
  // }
  // printf ("port = 0x%X, bsp_gpio_set()\n", (l[g]).gpios);
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
  // switch ((l[g]).gpioport)
  // {
  // case GPIOA:
  //   printf ("GPIOA, ");
  //   break;
  // case GPIOB:
  //   printf ("GPIOB, ");
  //   break;
  // case GPIOC:
  //   printf ("GPIOC, ");
  //   break;
  // case GPIOD:
  //   printf ("GPIOD, ");
  //   break;
  // }
  // printf ("port = 0x%X, bsp_gpio_reset()\n", (l[g]).gpios);
  gpio_clear ((l[g]).gpioport, (l[g]).gpios);
}

uint16_t bsp_gpio_port_read (gpio_state_struct l[], GPIO_LIST g)
{
  return gpio_port_read ((l[g]).gpioport);
}

bool bsp_gpio_pin_read (gpio_state_struct l[], GPIO_LIST g)
{
  uint16_t ret = bsp_gpio_port_read (l, g);
  uint16_t bit = (l[g]).gpios;
  if (0 != (ret & bit))
  {
    // printf ("0x%x & 0x%x != 0, bsp_gpio_pin_read, true\n", ret, bit);
    return true;
  }
  else
  {
    // printf ("0x%x & 0x%x == 0, bsp_gpio_pin_read, false\n", ret, bit);
    return false;
  }
}

void keys_drive_line (gpio_state_struct l[], GPIO_LIST gpios[], int size_of_gpios, int index)
{
  // int gpio = gpios[index];
  for (int i = 0; i < size_of_gpios; i++)
  {
    if (index == i)
    {
      bsp_gpio_clear (l, gpios[i]);    // ???????????
    }
    else
    {
      bsp_gpio_set (l, gpios[i]);    // ???????????
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
// bool read_key (gpio_state_struct l[], GPIO_LIST columns[], int COLUMN_SIZE, GPIO_LIST rows[], int ROW_SIZE, int
// index)
// {
//   // for (int i=0;i<COLUMN_SIZE;i++)
//   // {
//   //   for (int j=0;j<ROW_SIZE;j++)
//   //   {
//   //   }
//   // }
//   int column = index % ROW_SIZE;
//   int row    = index / ROW_SIZE;

//   // for (int i = 0; i < ROW_SIZE; i++)
//   // {
//   //   uint32_t row_value = 0;
//   //   // drive row
//   //   for (int j = 0; j < COLUMN_SIZE; j++)
//   //   {
//   //     row_value = row_value << 1;
//   //     row_value &= bsp_gpio_pin_read (l, columns[j]) ? 1 : 0;
//   //   }
//   // }

//   return bsp_gpio_pin_read (l, columns[column]);
// }

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 * @intput g_key_buf[KEYBOARD_ROW_NUMBER], g_usb_report_buf[HID_REPORT_SIZE];
 * @output g_key_buf[KEYBOARD_ROW_NUMBER], g_usb_report_buf[HID_REPORT_SIZE];
 */
void keyboard_generate_report (void)
{
  // const uint32_t key_buf[KEYBOARD_ROW_NUMBER];// = g_key_buf;
  // uint32_t p = key_buf;
  // memcpy(p, g_key_buf, sizeof(g_key_buf) * sizeof(g_key_buf[0]));

  int layer = 0;
  // clear USB report buffer
  for (int i = 0; i < HID_REPORT_SIZE; i++)
  {
    g_usb_report_buf_previous[i] = g_usb_report_buf[i];
    g_usb_report_buf[i]          = 0;
  }

  // more than 6 keys pressed?
  int cnt = keyboard_pressed_key_count();
  if (cnt > 6)
  {
    return;
  }
  // key pressed <= 6
  else
  {
    // check if FN pressed, side effect, read global variable
    if (0 != keyboard_is_fn_pressed())
    {
      layer = 1;
    }
    // else
    // {
    //   layer = 0;
    // }

    // modifier key pressed
    // Ctrl or Alt or Shift pressed
    // side effect, read global variable
    // all normal keys
    for (int i = 0; i < KEYBOARD_COLUMN_NUMBER; i++)
    {
      for (int j = 0; j < KEYBOARD_ROW_NUMBER; j++)
      {
        if (0 != (g_key_buf[i] & (1 << j)))    // key pressed
        {
          uint16_t report = g_key_lut[0][layer][i][j];

          // fn key is pressed
          if (0 != (report & 0x200))    // all fn keys are defined by 0x200 + index
          {
            ;
          }
          // modifier key pressed
          else if (0 != (report & 0x100))    // all modifiers are defined with 0x100 + Usage ID
          {
            // please note time consuming here
            switch (report)
            {
            case K_CTRL_LEFT:
              g_usb_report_buf[0] |= 1 << 0;
              break;    // ctrl_left
            case K_SHIFT_LEFT:
              g_usb_report_buf[0] |= 1 << 1;
              break;    // shift_left
            case K_ALT_LEFT:
              g_usb_report_buf[0] |= 1 << 2;
              break;    // alt_left
            case K_SUPER_LEFT:
              g_usb_report_buf[0] |= 1 << 3;
              break;    // super_left
            case K_CTRL_RIGHT:
              g_usb_report_buf[0] |= 1 << 4;
              break;    // ctrl_right
            case K_SHIFT_RIGHT:
              g_usb_report_buf[0] |= 1 << 5;
              break;    // shift_right
            case K_ALT_RIGHT:
              g_usb_report_buf[0] |= 1 << 6;
              break;    // alt_right
            case K_SUPER_RIGHT:
              g_usb_report_buf[0] |= 1 << 7;
              break;    // super_right
            default:
              break;
            }
          }
          else
          {
            for (int k = 2; k < HID_REPORT_SIZE; k++)
            {
              if (0 == g_usb_report_buf[k])    // no data in this byte
              {
                g_usb_report_buf[k] = g_key_lut[0][layer][i][j] % 256;
                break;
              }
            }
          }
        }
      }
    }
  }

  //****************************************************************************
  for (int i = 2; i < HID_REPORT_SIZE; i++)
  {
    uint16_t key = g_usb_report_buf[i];
    int found    = 0;

    // try to find difference between the previous buffer and the current buffer
    for (int j = 2; j < HID_REPORT_SIZE; j++)
    {
      // found = 0;
      if (key == g_usb_report_buf_previous[j])
      {
        found = 1;
        break;
      }
    }

    // not found, key rise detected
    if (0 == found)
    {
      // key_logger_add (key);
    }
  }

  return;
}

uint32_t keyboard_is_fn_pressed (void)
{
  for (int i = 0; i < KEYBOARD_COLUMN_NUMBER; i++)
  {
    if (0 != (g_key_buf[i] & g_fn_mask[i]))
    {
      return 1;    // pressed
    }
  }
  return 0;    // not pressed
}

/**
 * @brief  Check how many keys are pressed
 * @param  None
 * @retval number_keys
 * @intput g_key_buf[KEYBOARD_ROW_NUMBER]
 * @output
 */
int32_t keyboard_pressed_key_count (void)
{
  int32_t cnt;
  // very fast and cool method to count 1s in variable
  // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
  uint32_t v;                               // count bits set in this (32-bit value)
  uint32_t c;                               // store the total here
  const uint32_t S[] = {1, 2, 4, 8, 16};    // Magic Binary Numbers
  const uint32_t B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};

  cnt = 0;
  for (int i = 0; i < KEYBOARD_COLUMN_NUMBER; i++)
  {
    v = g_key_buf[i];
    c = v - ((v >> 1) & B[0]);
    c = ((c >> S[1]) & B[1]) + (c & B[1]);
    c = ((c >> S[2]) + c) & B[2];
    c = ((c >> S[3]) + c) & B[3];
    c = ((c >> S[4]) + c) & B[4];
    cnt += c;
  }
  return cnt;
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
