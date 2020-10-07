#!/bin/sh
RUNNING_PATH=${PWD}
RELATED_PATH=${0%/*}
cd ${RUNNING_PATH}
cd ${RELATED_PATH}

echo $PWD
# ls -alh ../usb_test/bbq10_keyboard.bin
# echo $PWD
set -x
# cd ..
st-flash write ../usb_test/bbq10_keyboard.bin 0x08000000
set +x