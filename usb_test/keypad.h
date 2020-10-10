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
#include "keys.h"
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

// the sequence should be exactly the same to gpio_state_struct g_gpio_state_list[]
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
// void bsp_gpio_init (gpio_state_struct l[], ssize_t length);
void bsp_gpio_init (gpio_state_struct l[]);
void bsp_gpio_toggle (gpio_state_struct l[], GPIO_LIST g);
void bsp_gpio_set (gpio_state_struct l[], GPIO_LIST g);
void bsp_gpio_clear (gpio_state_struct l[], GPIO_LIST g);
uint16_t bsp_gpio_port_read (gpio_state_struct l[], GPIO_LIST g);
bool bsp_gpio_pin_read (gpio_state_struct l[], GPIO_LIST g);

// void bsp_gpio_toggle (GPIO_LIST g);

bool read_key (gpio_state_struct l[], GPIO_LIST columns[], int column_size, GPIO_LIST rows[], int row_size, int index);
void keys_drive_line (gpio_state_struct l[], GPIO_LIST gpios[], int size_of_gpios, int index);

#endif /* __KEYPAD_H__ */

/************************ (C) COPYRIGHT ************************END OF FILE****/
