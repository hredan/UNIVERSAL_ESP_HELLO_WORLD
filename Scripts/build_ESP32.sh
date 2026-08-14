#!/usr/bin/bash
# to use the script, https://github.com/hredan/eep-build-action must be cloned before.
# set path in BUILD_ACTIOM_DIR to the path of the cloned repository.
# With GitBash the scripts are working also on Windows.

BUILD_ACTION_DIR="../eep-build-action/"
export INPUT_SKETCH_NAME="UNIVERSAL_ESP_HELLO_WORLD"
export INPUT_CORE="esp32"
export INPUT_BOARD="esp32" # ESP32 Dev Module
export INPUT_CORE_VERSION=""
export INPUT_CPU_F="160"
export INPUT_LIBS="Dictionary"

URL=$(git remote get-url origin)
HASH=$(git rev-parse HEAD)


# $BUILD_ACTION_DIR/build_sketch.sh -s $SKETCH_NAME -c $CORE -b $BOARD -f $CPU_F -l $LIBS
python3 $BUILD_ACTION_DIR/build_sketch.py
$BUILD_ACTION_DIR/create_build_info.sh -r $URL -s $HASH
$BUILD_ACTION_DIR/create_eep_package.sh -s $INPUT_SKETCH_NAME -c $INPUT_CORE -b $INPUT_BOARD
