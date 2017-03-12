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

class YablTest : public ::testing::Test
{
protected:
  virtual void SetUp() {
    CREATE_MOCK_ARDUINO();
  }
  
  virtual void TearDown() {
    DESTROY_MOCK_ARDUINO();
  }
  
  void SET_MILLIS(unsigned long millis) {
    ON_CALL(ARDUINO(), millis()).WillByDefault(Return(millis));
    button.previous_millis = _previousMillis;
    _previousMillis = millis;
  }
  
  void SET_READ_ROSE_FELL(bool read, bool rose, bool fell) {
    ON_CALL(button, read()).WillByDefault(Return(read));
    ON_CALL(button, rose()).WillByDefault(Return(rose));
    ON_CALL(button, fell()).WillByDefault(Return(fell));
  }
  
  Button button;

private:
  unsigned long _previousMillis = 0;
};


TEST_F(YablTest, initialUpState) {
  EXPECT_CALL(button, read())
    .WillRepeatedly(Return(true));

  EXPECT_TRUE(button.inverted());
  EXPECT_TRUE(button.read());
  EXPECT_FALSE(button.down());
  EXPECT_FALSE(button.activity());
}


TEST_F(YablTest, initialDownState) {
  EXPECT_CALL(button, read())
    .WillRepeatedly(Return(false));
  
  EXPECT_TRUE(button.inverted());
  EXPECT_FALSE(button.read());
  EXPECT_TRUE(button.down());
  EXPECT_FALSE(button.activity());
}


TEST_F(YablTest, pressRelease) {
  SET_MILLIS(100);
  SET_READ_ROSE_FELL(HIGH, false, false);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_FALSE(button.released());

  SET_MILLIS(200);
  SET_READ_ROSE_FELL(LOW, false, true);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.pressed());
  EXPECT_FALSE(button.released());

  SET_MILLIS(300);
  SET_READ_ROSE_FELL(LOW, false, false);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_FALSE(button.released());
  
  SET_MILLIS(400);
  SET_READ_ROSE_FELL(HIGH, true, false);
  EXPECT_TRUE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_TRUE(button.released());
  
  SET_MILLIS(500);
  SET_READ_ROSE_FELL(HIGH, false, false);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_FALSE(button.released());
}
