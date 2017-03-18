#!/usr/bin/env bash
[[ -d Arduino ]] || git clone https://github.com/arduino/Arduino.git
cd Arduino/build
ant clean dist
