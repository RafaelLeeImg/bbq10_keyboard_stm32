/**
 ******************************************************************************
 * @file
 * @author  Rafael Lee
 * @version v0.0.1
 * @date
 * @brief   Header for keypad.c module
 * From HID Usage Tables FOR USB v1.2
 * Chapter 10 Keyboard/Keypad Page (0x07)

 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEYS_H__
#define __KEYS_H__

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
// *****************************************************************************
// key layout defination
// last line: Fn, Ctrl, Super, Alt, blank, Space, Space, Space, Space, Insert, Alt, Ctrl, Fn, Left, Down, Right
//            0     1     2     3     4      5       6     7      8     10    Super  11   12    13   14    15

// ************************************
// https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2
// or refer to USB HIS usage table
// --------------------
#define NUMBER_OF_KEYS (91)
// --------------------
#define K_UNDEFINED (0)
#define K_F1        (0x3A)
#define K_F2        (0x3B)
#define K_F3        (0x3C)
#define K_F4        (0x3D)
#define K_F5        (0x3E)
#define K_F6        (0x3F)
#define K_F7        (0x40)
#define K_F8        (0x41)
#define K_F9        (0x42)
#define K_F10       (0x43)
#define K_F11       (0x44)
#define K_F12       (0x45)
// --------------------
#define K_ESC        (0x29)
#define K_BACK_QUOTE (0x35)    // modified according to cherry G83 keyboard
// --------------------
#define K_MINUS (0x2D)
#define K_EQUAL (0x2E)
// --------------------
#define K_DEL (0x4C)
// --------------------
#define K_SCROLL_LOCK (0x47)
#define K_TAB         (0x2B)
// --------------------
#define K_BRACKET_LEFT  (0x2F)
#define K_BRACKET_RIGHT (0X30)    // name longest 15 characters
#define K_BACK_SLASH    (0x31)
#define K_BACKSPACE     (0x2A)
// --------------------
#define K_PAUSE_BREAK (0x48)
// #define K_MUTE        (0x7F)    // in hid usage pdf ERROR
// #define K_VOL_UP      (0x80)    // in hid usage pdf ERROR
// #define K_VOL_DOWN    (0x81)    // in hid usage pdf ERROR
// --------------------
#define K_MUTE (0xE2)
// capture from USB keyboard whole report is 3 bytes 0x02 0xE2 0x00
#define K_VOL_UP        (0xE9)    // capture from USB keyboard
#define K_VOL_DOWN      (0xEA)    // capture from USB keyboard
#define K_PLAY_PREVIOUS (0xB6)
#define K_PLAY_PLAY     (0xCD)
#define K_PLAY_NEXT     (0xB5)
#define K_PLAY_STOP     (0xB7)
// --------------------
#define K_A (0x04)
#define K_B (0x05)
#define K_C (0x06)
#define K_D (0x07)
#define K_E (0x08)
#define K_F (0x09)
#define K_G (0x0A)
#define K_H (0x0B)
#define K_I (0x0C)
#define K_J (0x0D)
#define K_K (0x0E)
#define K_L (0x0F)
#define K_M (0x10)
#define K_N (0x11)
#define K_O (0x12)
#define K_P (0x13)
#define K_Q (0x14)
#define K_R (0x15)
#define K_S (0x16)
#define K_T (0x17)
#define K_U (0x18)
#define K_V (0x19)
#define K_W (0x1A)
#define K_X (0x1B)
#define K_Y (0x1C)
#define K_Z (0x1D)
// --------------------
#define K_1 (0x1E)
#define K_2 (0x1F)
#define K_3 (0x20)
#define K_4 (0x21)
#define K_5 (0x22)
#define K_6 (0x23)
#define K_7 (0x24)
#define K_8 (0x25)
#define K_9 (0x26)
#define K_0 (0x27)
// --------------------
#define K_SEMICOLON (0x33)
#define K_QUOTE     (0x34)
#define K_ENTER     (0x28)
#define K_HOME      (0x4A)
#define K_END       (0x4D)
// --------------------
#define K_PRTSC  (0x46)
#define K_SYSREQ (0x9A)
// --------------------
#define K_COMMA     (0x36)
#define K_PERIOD    (0x37)
#define K_SLASH     (0x38)
#define K_PAGE_UP   (0x4B)
#define K_UP        (0x52)
#define K_DOWN      (0x51)
#define K_LEFT      (0x50)
#define K_RIGHT     (0x4F)
#define K_PAGE_DOWN (0x4E)
#define K_MENU      (0x76)    // not used
#define K_APP       (0x65)
// --------------------
#define K_CAPS_LOCK (0x39)
#define K_NUM_LOCK  (0x53)
// --------------------
#define K_SPACE  (0x2C)
#define K_INSERT (0x49)
// --------------------
// all FN keys are defined as 0x201 + index, the first is 0x201
#define K_FN_LEFT  (0x201)
#define K_FN_RIGHT (0x202)
// --------------------
#define K_CTRL_LEFT   (0x1E0)
#define K_SHIFT_LEFT  (0x1E1)
#define K_ALT_LEFT    (0x1E2)
#define K_SUPER_LEFT  (0x1E3)
#define K_CTRL_RIGHT  (0X1E4)
#define K_SHIFT_RIGHT (0X1E5)
#define K_ALT_RIGHT   (0x1E6)
#define K_SUPER_RIGHT (0x1E7)

#endif /* __KEYS_H__ */

/************************ (C) COPYRIGHT ************************END OF FILE****/
