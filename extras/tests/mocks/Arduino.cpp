#include "Arduino.h"

ArduinoMock* _arduino = 0;

void SET_MOCK_ARDUINO(ArduinoMock& arduino) { _arduino = &arduino; }

unsigned long millis() { return _arduino ? _arduino->millis() : 0; }
