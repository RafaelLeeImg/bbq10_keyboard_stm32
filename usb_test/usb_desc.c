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

static const uint8_t hid_report_descriptor[] = {
  0x05, 0x01,    // Usage Page (Generic Desktop Ctrls)
  0x09, 0x06,    // Usage (Keyboard)
  0xA1, 0x01,    // Collection (Application)
  0x05, 0x07,    //   Usage Page (Kbrd/Keypad)
  0x19, 0xE0,    //   Usage Minimum (0xE0)
  0x29, 0xE7,    //   Usage Maximum (0xE7)
  0x15, 0x00,    //   Logical Minimum (0)
  0x25, 0x01,    //   Logical Maximum (1)
  0x95, 0x08,    //   Report Count (8)
  0x75, 0x01,    //   Report Size (1)
  0x81, 0x02,    //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x95, 0x01,    //   Report Count (1)
  0x75, 0x08,    //   Report Size (8)
  0x81, 0x03,    //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x95, 0x06,    //   Report Count (6)
  0x75, 0x08,    //   Report Size (8)
  0x25, 0xFF,    //   Logical Maximum (-1)
  0x19, 0x00,    //   Usage Minimum (0x00)
  0x29, 0x65,    //   Usage Maximum (0x65)
  0x81, 0x00,    //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x25, 0x01,    //   Logical Maximum (1)
  0x95, 0x02,    //   Report Count (2)
  0x75, 0x01,    //   Report Size (1)
  0x05, 0x08,    //   Usage Page (LEDs)
  0x19, 0x01,    //   Usage Minimum (Num Lock)
  0x29, 0x02,    //   Usage Maximum (Caps Lock)
  0x91, 0x02,    //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x95, 0x01,    //   Report Count (1)
  0x75, 0x06,    //   Report Size (6)
  0x91, 0x03,    //   Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,          // End Collection

  // 61 bytes
  // http://eleccelerator.com/usbdescreqparser/
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
  .wMaxPacketSize   = 8,
  .bInterval        = 0x20,
};

const struct usb_interface_descriptor hid_iface = {
  .bLength            = USB_DT_INTERFACE_SIZE,
  .bDescriptorType    = USB_DT_INTERFACE,
  .bInterfaceNumber   = 0,
  .bAlternateSetting  = 0,
  .bNumEndpoints      = 1,
  .bInterfaceClass    = USB_CLASS_HID,
  .bInterfaceSubClass = 1,
  .bInterfaceProtocol = 6, /* Keyboard */
  .iInterface         = 0,
  // .bInterfaceProtocol error does not lead to malfunction
  // HID Usage Tables FOR USB v1.2
  // Table 4.1: Generic Desktop Page

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
  "SZDIY",
  "BlackBerry Q10 Keyboard",
  "0",
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

  usbd_ep_setup (dev, hid_endpoint.bEndpointAddress, hid_endpoint.bmAttributes, hid_endpoint.wMaxPacketSize, NULL);

  usbd_register_control_callback (dev, USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
                                  USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, hid_control_request);
#ifdef INCLUDE_DFU_INTERFACE
  usbd_register_control_callback (dev, USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
                                  USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, dfu_control_request);
#endif

  /* SysTick interrupt every N clock pulses: set reload to N-1 */
  // choose clk source STK_CSR_CLKSOURCE_AHB or STK_CSR_CLKSOURCE_AHB_DIV8 automatically
  // uint32_t freq, uint32_t frequency_of_AHB
  systick_set_frequency (1000, 72000000);
  systick_interrupt_enable();
  systick_counter_enable();
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
