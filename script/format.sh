#!/bin/sh
RUNNING_PATH=${PWD}
RELATED_PATH=${0%/*}
cd ${RUNNING_PATH}
cd ${RELATED_PATH}

# clang-format -i --verbose ../usb_test/my-project.c
# clang-format -i --verbose ../usb_test/bsp_config.h
clang-format -i --verbose ../usb_test/bsp_config.h
clang-format -i --verbose ../usb_test/main.c
clang-format -i --verbose ../usb_test/usb_desc.c
clang-format -i --verbose ../usb_test/usb_desc.h
clang-format -i --verbose ../usb_test/keypad.c
clang-format -i --verbose ../usb_test/keypad.h
clang-format -i --verbose ../usb_test/interrupt.c
clang-format -i --verbose ../usb_test/interrupt.h
## clang-format -i --verbose

