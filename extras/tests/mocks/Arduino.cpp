/*
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 */

#include "Arduino.h"

ArduinoMock* _arduino = 0;

void SET_MOCK_ARDUINO(ArduinoMock& arduino) { _arduino = &arduino; }

unsigned long millis() { return _arduino ? _arduino->millis() : 0; }
