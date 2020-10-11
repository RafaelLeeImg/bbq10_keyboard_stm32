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

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

// #include "bsp_config.h"
#include "api-asm.h"
#include "api.h"
#include "bsp_config.h"
#include "interrupt.h"
#include "keypad.h"
#include "usb_desc.h"
/* Private typedef -----------------------------------------------------------*/

extern char usb_serial_number[25];                      /* 12 bytes of desig and a \0 */
extern const struct usb_device_descriptor dev_descr;    // usb_desc.c
// extern const struct usb_config_descriptor config_desc_0;
extern const struct usb_config_descriptor config;
extern const char* usb_strings[];
extern uint8_t usbd_control_buffer[128];
extern gpio_state_struct g_gpio_state_list[];
extern void hid_set_config (usbd_device* dev, uint16_t wValue);
extern usbd_device* usbd_dev;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void sys_tick_handler (void);
// extern void usbmidi_set_config (usbd_device* usbd_dev, uint16_t wValue);
extern void button_poll (usbd_device* usbd_dev);
static void button_send_event (usbd_device* usbd_dev, int pressed);
static void usart_setup (void);
/* Private functions ---------------------------------------------------------*/

// TX only
static void usart_setup (void)
{

  // /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  // gpio_set_mode (GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

  // /* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
  // gpio_set_mode (GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

  /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
  gpio_set_mode (GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
  gpio_set_mode (GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
  gpio_set (GPIOA, GPIO_USART1_RX);

  /* Setup UART parameters. */
  usart_set_baudrate (USART1, 115200);
  usart_set_databits (USART1, 8);
  usart_set_stopbits (USART1, USART_STOPBITS_1);
  usart_set_parity (USART1, USART_PARITY_NONE);
  usart_set_flow_control (USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode (USART1, USART_MODE_TX_RX);
  // usart_set_mode (USART1, USART_MODE_TX);

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
  // usbd_device* usbd_dev;

  // rcc_clock_setup_in_hse_8mhz_out_72mhz();
  rcc_clock_setup_in_hse_16mhz_out_72mhz();

  rcc_set_usbpre (RCC_CFGR_USBPRE_PLL_CLK_DIV1_5);

  rcc_periph_clock_enable (RCC_GPIOA);    // usb, usart
  rcc_periph_clock_enable (RCC_GPIOB);    // I2c, column, row, mic
  // rcc_periph_clock_enable(RCC_GPIOD); // crystal
  rcc_periph_clock_enable (RCC_AFIO);
  // // rcc_periph_clock_enable(RCC_OTGFS);
  // rcc_periph_clock_enable(RCC_USB);
  rcc_periph_clock_enable (RCC_USART1);
  tim2_interrupt_setup();

  // to set PA15 free from debug port
  // avoid PA15 in next version
  AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON;

  bsp_gpio_init (g_gpio_state_list);

  // set all row GPIOs to pull-up
  // GPIO_LIST rows[] = {KEY_ROW_1, KEY_ROW_2, KEY_ROW_3, KEY_ROW_4, KEY_ROW_5, KEY_ROW_6, KEY_ROW_7};
  int row_size = sizeof (rows) / sizeof (GPIO_LIST);
  for (int i = 0; i < row_size; i++)
  {
    bsp_gpio_set (g_gpio_state_list, rows[i]);
  }

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

  gpio_clear (LED0_PORT, LED0_PIN);
  gpio_toggle (LED0_PORT, LED0_PIN);

  // printf ("Hello World! %i %f %f\r\n", counter, fcounter, dcounter);
  printf ("Hello World! %i %f %f\r\n", 3, 1.3, 2.3);
  gpio_toggle (LED0_PORT, LED0_PIN);

  // for (int j = 0; j < 10; j++)
  // {
  //   gpio_toggle (LED0_PORT, LED0_PIN);
  //   for (unsigned i = 0; i < 72000000; i++)
  //   {
  //     __asm__("nop");
  //   }
  // }

  /*
   * This is a somewhat common cheap hack to trigger device re-enumeration
   * on startup.  Assuming a fixed external pullup on D+, (For USB-FS)
   * setting the pin to output, and driving it explicitly low effectively
   * "removes" the pullup.  The subsequent USB init will "take over" the
   * pin, and it will appear as a proper pullup to the host.
   * The magic delay is somewhat arbitrary, no guarantees on USBIF
   * compliance here, but "it works" in most places.
   */
  gpio_set_mode (GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
  gpio_clear (GPIOA, GPIO12);
  // for (unsigned i = 0; i < 800000; i++)
  for (unsigned i = 0; i < 800000; i++)
  {
    __asm__("nop");
  }

  usbd_dev = usbd_init (&st_usbfs_v1_usb_driver, &dev_descr, &config, usb_strings, 3, usbd_control_buffer,
                        sizeof (usbd_control_buffer));
  usbd_register_set_config_callback (usbd_dev, hid_set_config);

  while (1)
  {
    usbd_poll (usbd_dev);
    // button_poll (usbd_dev);
  }

  /* add your own code */
  uint32_t rev = 0xaabbccdd;
  rev          = rev_bytes (rev);
  return my_func (rev);
}

void button_poll (usbd_device* dev)
{
  static uint32_t button_state = 0;

  /* This is a simple shift based debounce. It's simplistic because
   * although this implements debounce adequately it does not have any
   * noise suppression. It is also very wide (32-bits) because it can
   * be polled in a very tight loop (no debounce timer).
   */
  // uint32_t old_button_state = button_state;
  // button_state              = (button_state << 1) | (GPIOA_IDR & 1);
  // if ((0 == button_state) != (0 == old_button_state))
  // {
  //   button_send_event (dev, !!button_state);
  // }
}

static void button_send_event (usbd_device* dev, int pressed)
{
  char buf[4] = {
    0x08, /* USB framing: virtual cable 0, note on */
    0x80, /* MIDI command: note on, channel 1 */
    60,   /* Note 60 (middle C) */
    64,   /* "Normal" velocity */
  };

  buf[0] |= pressed;
  buf[1] |= pressed << 4;

  while (usbd_ep_write_packet (dev, 0x81, buf, sizeof (buf)) == 0)
    ;
}

void sys_tick_handler (void)
{
  static unsigned int x = 0;
  static int dir        = 1;
  uint8_t buf[8]        = {0, 0, 0, 0, 0, 0, 0, 0};

  buf[2] = 0;
  x++;
  if (x == 400)
  {
    buf[2] = 0x11;    // key n
    // while (usbd_ep_write_packet (usbd_dev, 0x81, buf, sizeof (buf) / sizeof (uint8_t)) == 0) { ; }
  }
  else if (x >= 500)
  {
    bsp_gpio_toggle (g_gpio_state_list, LED0);
    buf[2] = 0;
    // while (usbd_ep_write_packet (usbd_dev, 0x81, buf, sizeof (buf) / sizeof (uint8_t)) == 0) { ; }
    x = 0;
  }
  else
  {
  }    // do nothing
}

/************************ (C) COPYRIGHT ************************END OF FILE****/
