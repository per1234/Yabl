/*
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 */

#include <Yabl.h>
#include <Arduino.h>
#include <gtest/gtest.h>

using ::testing::Return;

TEST(YablTest, initialUpState) {
  Button button;
  
  EXPECT_CALL(button, read())
    .WillRepeatedly(Return(true));

  EXPECT_TRUE(button.inverted());
  EXPECT_TRUE(button.read());
  EXPECT_FALSE(button.down());
  EXPECT_FALSE(button.activity());
}

TEST(YablTest, initialDownState) {
  Button button;
  
  EXPECT_CALL(button, read())
    .WillRepeatedly(Return(false));
  
  EXPECT_TRUE(button.inverted());
  EXPECT_FALSE(button.read());
  EXPECT_TRUE(button.down());
  EXPECT_FALSE(button.activity());
}

TEST(YablTest, updateCallsMillis) {
  DECLARE_MOCK_ARDUINO(Arduino);
  
  Button button;
  
  EXPECT_CALL(Arduino, millis())
    .WillOnce(Return(10));
  
  EXPECT_FALSE(button.update());
}
