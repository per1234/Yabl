/*
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 */

#pragma once

#include "gmock/gmock.h"

class ArduinoMock
{
public:
  MOCK_METHOD0(millis, unsigned long());
};

#define DECLARE_MOCK_ARDUINO(_mockArduino) \
  ArduinoMock _mockArduino; \
  SET_MOCK_ARDUINO(_mockArduino);

void SET_MOCK_ARDUINO(ArduinoMock& arduino);

unsigned long millis();
