#!/usr/bin/env bash
[[ -d Arduino ]] || git clone https://github.com/arduino/Arduino.git
cd Arduino/build
#echo 1.8.2 | ant clean dist
if [ "$(uname)" == "Darwin" ]; then
  arduino_binary=`pwd`/macosx/work/Arduino.app/Contents/MacOS/Arduino
else
  arduino_binary=`pwd`/linux/work/arduino
fi
# this doesn't work for some reason
# $arduino_binary --install-library "Bounce2"
cd ../../libraries
git clone https://github.com/thomasfredericks/Bounce2.git
