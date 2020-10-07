/**
 ******************************************************************************
 * @file
 * @author  Rafael Lee
 * @version v0.0.1
 * @date
 * @brief   main.c
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <errno.h>    // EIO
#include <stdint.h>
#include <stdio.h>
// #include <libopencm3/usb/audio.h>
// #include <libopencm3/usb/midi.h>
// #include <libopencm3/usb/usbd.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

// #include "bsp_config.h"
#include "api-asm.h"
#include "api.h"
#include "bsp_config.h"
#include "keypad.h"
#include "usb_desc.h"
/* Private typedef -----------------------------------------------------------*/

extern char usb_serial_number[25];                       /* 12 bytes of desig and a \0 */
extern const struct usb_device_descriptor dev_desc_0;    // usb_desc.c
extern const struct usb_config_descriptor config_desc_0;
extern const char* usb_strings[];
extern uint8_t usbd_control_buffer[128];

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
// #define NULL (void*)(0)
#define NULL (0)
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
// extern void usbmidi_set_config (usbd_device* usbd_dev, uint16_t wValue);
extern void button_poll (usbd_device* usbd_dev);
static void button_send_event (usbd_device* usbd_dev, int pressed);
static void usart_setup (void);
/* Private functions ---------------------------------------------------------*/

// TX only
static void usart_setup (void)
{
  /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  gpio_set_mode (GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
  // gpio_set_mode (GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_USART1_TX);

  /* Setup UART parameters. */
  usart_set_baudrate (USART1, 115200);
  usart_set_databits (USART1, 8);
  usart_set_stopbits (USART1, USART_STOPBITS_1);
  usart_set_parity (USART1, USART_PARITY_NONE);
  usart_set_flow_control (USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode (USART1, USART_MODE_TX);

  /* Finally enable the USART. */
  usart_enable (USART1);
}

int _write (int file, char* ptr, int len)
{
  int i;

  if (file == 1)
  {
    for (i = 0; i < len; i++)
      usart_send_blocking (USART1, ptr[i]);
    return i;
  }

  errno = EIO;
  return -1;
}

/**
 * \par Function
 *   dr::split_by_delimiters
 * \par Description
 *   Split string by delimiters.
 * \par Output
 *   None
 * \par return
 *   std::vector<string> str_vect
 * \par Others
 *
 */
int main (void)
{
  usbd_device* usbd_dev;

  // rcc_clock_setup_in_hse_8mhz_out_72mhz();
  rcc_clock_setup_in_hse_16mhz_out_72mhz();

  rcc_set_usbpre (RCC_CFGR_USBPRE_PLL_CLK_DIV1_5);

  rcc_periph_clock_enable (RCC_GPIOA);    // usb, usart
  // rcc_periph_clock_enable(RCC_GPIOD); // crystal
  rcc_periph_clock_enable (RCC_AFIO);
  // // rcc_periph_clock_enable(RCC_OTGFS);
  // rcc_periph_clock_enable(RCC_USB);
  rcc_periph_clock_enable (RCC_USART1);

  // // gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10 | GPIO11 | GPIO12);

  // /* Setup GPIOA, PIN9,10,11,12 (in GPIO port A) for devic use. */
  // // PA9     ------> USART1_TX
  // gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
  // // PA10     ------> USART1_RX
  // gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO10);

  // gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO11|GPIO12);

  // // USB D- pull-up to 3.3V with 1.5k resistor for full speed device
  // // USB D+ pull-up to 3.3V with 1.5k resistor for high speed device
  usart_setup();

  gpio_set_mode (LED0_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED0_PIN);
  gpio_clear (LED0_PORT, LED0_PIN);
  gpio_toggle (LED0_PORT, LED0_PIN);

  // printf ("Hello World! %i %f %f\r\n", counter, fcounter, dcounter);
  printf ("Hello World! %i %f %f\r\n", 3, 1.3, 2.3);

  // for (int j = 0; j < 10; j++)
  // {
  //   gpio_toggle (LED0_PORT, LED0_PIN);
  //   for (unsigned i = 0; i < 72000000; i++)
  //   {
  //     __asm__("nop");
  //   }
  // }

  usbd_dev = usbd_init (&st_usbfs_v1_usb_driver, &dev_desc_0, &config_desc_0, usb_strings, 3, usbd_control_buffer,
                        sizeof (usbd_control_buffer));

  usbd_register_set_config_callback (usbd_dev, usbmidi_set_config);

  while (1)
  {
    usbd_poll (usbd_dev);
    button_poll (usbd_dev);
  }

  /* add your own code */
  uint32_t rev = 0xaabbccdd;
  rev          = rev_bytes (rev);
  return my_func (rev);
}

void button_poll (usbd_device* usbd_dev)
{
  static uint32_t button_state = 0;

  /* This is a simple shift based debounce. It's simplistic because
   * although this implements debounce adequately it does not have any
   * noise suppression. It is also very wide (32-bits) because it can
   * be polled in a very tight loop (no debounce timer).
   */
  uint32_t old_button_state = button_state;
  button_state              = (button_state << 1) | (GPIOA_IDR & 1);
  if ((0 == button_state) != (0 == old_button_state))
  {
    button_send_event (usbd_dev, !!button_state);
  }
}

static void button_send_event (usbd_device* usbd_dev, int pressed)
{
  char buf[4] = {
    0x08, /* USB framing: virtual cable 0, note on */
    0x80, /* MIDI command: note on, channel 1 */
    60,   /* Note 60 (middle C) */
    64,   /* "Normal" velocity */
  };

  buf[0] |= pressed;
  buf[1] |= pressed << 4;

  while (usbd_ep_write_packet (usbd_dev, 0x81, buf, sizeof (buf)) == 0)
    ;
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
