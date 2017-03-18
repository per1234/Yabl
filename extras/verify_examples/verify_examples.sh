#!/usr/bin/env bash
if [ "$(uname)" == "Darwin" ]; then
  arduino_root=`pwd`/Arduino/build/macosx/work/Arduino.app/Contents/Java
else
  arduino_root=`pwd`/Arduino/build/linux/work/Arduino.app/Contents/Java
fi

if [ ! -d $arduino_root ]; then
  echo "Arduino not installed! Run install_arduino.sh"
  exit 1
fi

build_root=`pwd`/build

for sketch in `find ../../examples -name '*.ino'`
do
  echo $sketch
  sketch_build_dir=$(basename $sketch)
  mkdir -p $build_root/$sketch_build_dir

  cd $arduino_root
  ./arduino-builder -hardware ./hardware -tools ./hardware/tools/avr -tools ./tools-builder -libraries ./libraries -libraries $build_root/../libraries -fqbn arduino:avr:uno -build-path $build_root/$sketch_build_dir $build_root/../$sketch
done
