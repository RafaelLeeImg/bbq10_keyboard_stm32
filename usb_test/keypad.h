/**
 ******************************************************************************
 * @file
 * @author  Rafael Lee
 * @version v0.0.1
 * @date
 * @brief   Header for keypad.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEYPAD_H__
#define __KEYPAD_H__

/* Includes ------------------------------------------------------------------*/
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t gpioport;
  uint16_t gpios;
  uint16_t reset_state;
  uint8_t mode;
  uint8_t config;
} gpio_state_struct;    // TODO:rename to gpio_state_conf

typedef enum
{
  KEY_ROW_1 = 0,
  KEY_ROW_2,
  KEY_ROW_3,
  KEY_ROW_4,
  KEY_ROW_5,
  KEY_ROW_6,
  KEY_ROW_7,
  KEY_COL_1,
  KEY_COL_2,
  KEY_COL_3,
  KEY_COL_4,
  KEY_COL_5,
  LED0,
  BACK_LIGHT0,
  EMPTY,
} GPIO_LIST;    // TODO: rename

/* Exported constants --------------------------------------------------------*/
// gpio_state_struct g_gpio_state_list[];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void bsp_gpio_init (gpio_state_struct l[]);
void bsp_gpio_toggle (gpio_state_struct l[], GPIO_LIST g);
void bsp_gpio_set (gpio_state_struct l[], GPIO_LIST g);
void bsp_gpio_reset (gpio_state_struct l[], GPIO_LIST g);
// void bsp_gpio_toggle (GPIO_LIST g);
#endif /* __KEYPAD_H__ */

/************************ (C) COPYRIGHT ************************END OF FILE****/
