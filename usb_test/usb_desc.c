/**
 ******************************************************************************
 * @file
 * @author  Rafael Lee
 * @version v0.0.1
 * @date
 * @brief   usb_desc.c
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"

// #include <libopencm3/usb/audio.h>
// #include <libopencm3/usb/midi.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/usb/hid.h>
#include <libopencm3/usb/usbstd.h>

#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Define this to include the DFU APP interface. */
// #define INCLUDE_DFU_INTERFACE

#ifdef INCLUDE_DFU_INTERFACE
#include <libopencm3/cm3/scb.h>
#include <libopencm3/usb/dfu.h>
#endif

usbd_device* usbd_dev;

const struct usb_device_descriptor dev_descr = {
  .bLength            = USB_DT_DEVICE_SIZE,
  .bDescriptorType    = USB_DT_DEVICE,
  .bcdUSB             = 0x0200, /* was 0x0110 in Table B-1 example descriptor */
  .bDeviceClass       = 0,      /* device defined at interface level */
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = 64,
  .idVendor           = 0x0483,
  .idProduct          = 0xffff,
  .bcdDevice          = 0x0200,
  .iManufacturer      = 1, /* index to string desc */
  .iProduct           = 2, /* index to string desc */
  .iSerialNumber      = 3, /* index to string desc */
  .bNumConfigurations = 1,
};

const struct usb_device_qualifier_descriptor dev_qualifier_desc = {
  // struct usb_device_qualifier_descriptor
  .bLength            = 0x0A,                    /* 0x0A */
  .bDescriptorType    = USB_DT_DEVICE_QUALIFIER, /* 6 */
  .bcdUSB             = 0x0200,                  /* a */
  .bDeviceClass       = 0,                       /* b */
  .bDeviceSubClass    = 0,                       /* c */
  .bDeviceProtocol    = 0,                       /* d */
  .bMaxPacketSize0    = 64,                      /* e */
  .bNumConfigurations = 1,                       /* f */
  .bReserved          = 0,                       /* reserved */
};

static const uint8_t hid_report_descriptor[] = {
  0x05, 0x01, /* USAGE_PAGE (Generic Desktop)         */
  0x09, 0x02, /* USAGE (Mouse)                        */
  0xa1, 0x01, /* COLLECTION (Application)             */
  0x09, 0x01, /*   USAGE (Pointer)                    */
  0xa1, 0x00, /*   COLLECTION (Physical)              */
  0x05, 0x09, /*     USAGE_PAGE (Button)              */
  0x19, 0x01, /*     USAGE_MINIMUM (Button 1)         */
  0x29, 0x03, /*     USAGE_MAXIMUM (Button 3)         */
  0x15, 0x00, /*     LOGICAL_MINIMUM (0)              */
  0x25, 0x01, /*     LOGICAL_MAXIMUM (1)              */
  0x95, 0x03, /*     REPORT_COUNT (3)                 */
  0x75, 0x01, /*     REPORT_SIZE (1)                  */
  0x81, 0x02, /*     INPUT (Data,Var,Abs)             */
  0x95, 0x01, /*     REPORT_COUNT (1)                 */
  0x75, 0x05, /*     REPORT_SIZE (5)                  */
  0x81, 0x01, /*     INPUT (Cnst,Ary,Abs)             */
  0x05, 0x01, /*     USAGE_PAGE (Generic Desktop)     */
  0x09, 0x30, /*     USAGE (X)                        */
  0x09, 0x31, /*     USAGE (Y)                        */
  0x09, 0x38, /*     USAGE (Wheel)                    */
  0x15, 0x81, /*     LOGICAL_MINIMUM (-127)           */
  0x25, 0x7f, /*     LOGICAL_MAXIMUM (127)            */
  0x75, 0x08, /*     REPORT_SIZE (8)                  */
  0x95, 0x03, /*     REPORT_COUNT (3)                 */
  0x81, 0x06, /*     INPUT (Data,Var,Rel)             */
  0xc0,       /*   END_COLLECTION                     */
  0x09, 0x3c, /*   USAGE (Motion Wakeup)              */
  0x05, 0xff, /*   USAGE_PAGE (Vendor Defined Page 1) */
  0x09, 0x01, /*   USAGE (Vendor Usage 1)             */
  0x15, 0x00, /*   LOGICAL_MINIMUM (0)                */
  0x25, 0x01, /*   LOGICAL_MAXIMUM (1)                */
  0x75, 0x01, /*   REPORT_SIZE (1)                    */
  0x95, 0x02, /*   REPORT_COUNT (2)                   */
  0xb1, 0x22, /*   FEATURE (Data,Var,Abs,NPrf)        */
  0x75, 0x06, /*   REPORT_SIZE (6)                    */
  0x95, 0x01, /*   REPORT_COUNT (1)                   */
  0xb1, 0x01, /*   FEATURE (Cnst,Ary,Abs)             */
  0xc0        /* END_COLLECTION                       */
};

static const struct
{
  struct usb_hid_descriptor hid_descriptor;
  struct
  {
    uint8_t bReportDescriptorType;
    uint16_t wDescriptorLength;
  } __attribute__ ((packed)) hid_report;
} __attribute__((packed)) hid_function = {
  .hid_descriptor = {
    .bLength = sizeof(hid_function),
    .bDescriptorType = USB_DT_HID,
    .bcdHID = 0x0100,
    .bCountryCode = 0,
    .bNumDescriptors = 1,
  },
  .hid_report = {
    .bReportDescriptorType = USB_DT_REPORT,
    .wDescriptorLength = sizeof(hid_report_descriptor),
  }
};

const struct usb_endpoint_descriptor hid_endpoint = {
  .bLength          = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType  = USB_DT_ENDPOINT,
  .bEndpointAddress = 0x81,
  .bmAttributes     = USB_ENDPOINT_ATTR_INTERRUPT,
  .wMaxPacketSize   = 4,
  .bInterval        = 0x20,
};

const struct usb_interface_descriptor hid_iface = {
  .bLength            = USB_DT_INTERFACE_SIZE,
  .bDescriptorType    = USB_DT_INTERFACE,
  .bInterfaceNumber   = 0,
  .bAlternateSetting  = 0,
  .bNumEndpoints      = 1,
  .bInterfaceClass    = USB_CLASS_HID,
  .bInterfaceSubClass = 1, /* boot */
  .bInterfaceProtocol = 2, /* mouse */
  .iInterface         = 0,

  .endpoint = &hid_endpoint,

  .extra    = &hid_function,
  .extralen = sizeof (hid_function),
};

const struct usb_interface ifaces[] = {{
  .num_altsetting = 1,
  .altsetting     = &hid_iface,

}};

const struct usb_config_descriptor config = {
  .bLength             = USB_DT_CONFIGURATION_SIZE,
  .bDescriptorType     = USB_DT_CONFIGURATION,
  .wTotalLength        = 0,
  .bNumInterfaces      = 1,
  .bConfigurationValue = 1,
  .iConfiguration      = 0,
  .bmAttributes        = 0xC0,
  .bMaxPower           = 0x32,

  .interface = ifaces,
};

const char* usb_strings[] = {
  "Black Sphere Technologies",
  "HID Demo",
  "DEMO",
};

/* Buffer to be used for control requests. */
uint8_t usbd_control_buffer[128];

/* Private function prototypes -----------------------------------------------*/
void hid_set_config (usbd_device* dev, uint16_t wValue);
/* Private functions ---------------------------------------------------------*/
static enum usbd_request_return_codes hid_control_request (usbd_device* dev, struct usb_setup_data* req, uint8_t** buf,
                                                           uint16_t* len,
                                                           void (**complete) (usbd_device*, struct usb_setup_data*))
{
  (void)complete;
  (void)dev;

  if ((req->bmRequestType != 0x81) || (req->bRequest != USB_REQ_GET_DESCRIPTOR) || (req->wValue != 0x2200))
    return USBD_REQ_NOTSUPP;

  /* Handle the HID report descriptor. */
  *buf = (uint8_t*)hid_report_descriptor;
  *len = sizeof (hid_report_descriptor);

  return USBD_REQ_HANDLED;
}

#ifdef INCLUDE_DFU_INTERFACE
static void dfu_detach_complete (usbd_device* dev, struct usb_setup_data* req)
{
  (void)req;
  (void)dev;

  gpio_set_mode (GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);
  gpio_set (GPIOA, GPIO10);
  scb_reset_core();
}

static enum usbd_request_return_codes dfu_control_request (usbd_device* dev, struct usb_setup_data* req, uint8_t** buf,
                                                           uint16_t* len,
                                                           void (**complete) (usbd_device*, struct usb_setup_data*))
{
  (void)buf;
  (void)len;
  (void)dev;

  if ((req->bmRequestType != 0x21) || (req->bRequest != DFU_DETACH))
    return USBD_REQ_NOTSUPP; /* Only accept class request. */

  *complete = dfu_detach_complete;

  return USBD_REQ_HANDLED;
}
#endif

void hid_set_config (usbd_device* dev, uint16_t wValue)
{
  (void)wValue;
  (void)dev;

  usbd_ep_setup (dev, 0x81, USB_ENDPOINT_ATTR_INTERRUPT, 4, NULL);

  usbd_register_control_callback (dev, USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
                                  USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, hid_control_request);
#ifdef INCLUDE_DFU_INTERFACE
  usbd_register_control_callback (dev, USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
                                  USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, dfu_control_request);
#endif

  systick_set_clocksource (STK_CSR_CLKSOURCE_AHB_DIV8);
  /* SysTick interrupt every N clock pulses: set reload to N-1 */
  systick_set_reload (99999);
  systick_interrupt_enable();
  systick_counter_enable();
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
