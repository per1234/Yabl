/*
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 */

#pragma once

#include "gmock/gmock.h"

class Bounce
{
public:
  MOCK_METHOD1(attach, void(int pin));
  MOCK_METHOD2(attach, void(int pin, int mode));
  MOCK_METHOD0(update, bool());
  MOCK_METHOD0(read, bool());
  MOCK_METHOD0(rose, bool());
  MOCK_METHOD0(fell, bool());
  
protected:
  unsigned long previous_millis = 0;
  int pin = 0;
};
