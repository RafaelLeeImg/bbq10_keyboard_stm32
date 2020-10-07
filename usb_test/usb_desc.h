/**
 ******************************************************************************
 * @file
 * @author  Rafael Lee
 * @version v0.0.1
 * @date
 * @brief   Header for xxx.c module
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
// #include <stdint.h>

#include "usb_desc.h"
#include <libopencm3/usb/usbd.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void usbmidi_set_config (usbd_device* usbd_dev, uint16_t wValue);

#endif /* __USB_DESC_H */

/************************ (C) COPYRIGHT ************************END OF FILE****/
