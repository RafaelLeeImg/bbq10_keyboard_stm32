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
#ifndef __USB_DESC_H__
#define __USB_DESC_H__

/* Includes ------------------------------------------------------------------*/
// #include <stdint.h>

#include <libopencm3/usb/usbd.h>
#include <stddef.h>    // NULL

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
// bool g_usb_initialized;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void usbmidi_set_config (usbd_device* usbd_dev, uint16_t wValue);

#endif /* __USB_DESC_H__ */

/************************ (C) COPYRIGHT ************************END OF FILE****/
