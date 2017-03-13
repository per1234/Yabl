/*
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 */

#pragma once

#include "gmock/gmock.h"
#include <Yabl.h>

class ButtonCallback
{
public:
  MOCK_METHOD0(callbackSimple, void());
  MOCK_METHOD2(callbackWithEventInfo, void(Button& button, Event event));
};
