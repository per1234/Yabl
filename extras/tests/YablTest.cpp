/*
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 */

#include <Yabl.h>
#include <Arduino.h>
#include <ButtonCallback.h>
#include <gtest/gtest.h>

using ::testing::Return;
using ::testing::AnyNumber;
using ::testing::Ref;
using ::testing::Mock;
using ::testing::InSequence;

class YablTest : public ::testing::Test
{
protected:
  virtual void SetUp() {
    CREATE_MOCK_ARDUINO();
    callbackPtr = &callback;
    _pinLevel = button.inverted();
  }
  
  virtual void TearDown() {
    DESTROY_MOCK_ARDUINO();
  }
  
  void SET_PIN_LEVEL_AT_MILLIS(bool level, unsigned long millis) {
    button.previous_millis = _previousMillis;
    _previousMillis = level != _pinLevel ? millis : _previousMillis;

    EXPECT_CALL(ARDUINO(), millis()).Times(AnyNumber()).WillRepeatedly(Return(millis));
    EXPECT_CALL(button, update()).WillOnce(Return(level != _pinLevel));
    ON_CALL(button, read()).WillByDefault(Return(level));
    ON_CALL(button, rose()).WillByDefault(Return(_pinLevel == LOW && level == HIGH));
    ON_CALL(button, fell()).WillByDefault(Return(_pinLevel == HIGH && level == LOW));

    _pinLevel = level;
    _millis = millis;
  }
  
  void VERIFY_AND_CLEAR() {
    Mock::VerifyAndClear(&button);
    Mock::VerifyAndClear(&callback);
    Mock::VerifyAndClear(&ARDUINO());
  }
  
  static void callbackSimple() {
    callbackPtr->callbackSimple();
  }
  
  static void callbackWithEventInfo(Button& button, Event event) {
    callbackPtr->callbackWithEventInfo(button, event);
  }
  
  ButtonCallback callback;
  Button button;
  
private:
  static ButtonCallback* callbackPtr;
  
  bool _pinLevel = false;
  unsigned long _millis = 0;
  unsigned long _previousMillis = 0;
};

ButtonCallback* YablTest::callbackPtr = 0;

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
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 100);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_FALSE(button.released());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(LOW, 200);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.pressed());
  EXPECT_FALSE(button.released());
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(LOW, 300);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_FALSE(button.released());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 400);
  EXPECT_TRUE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_TRUE(button.released());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 500);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.pressed());
  EXPECT_FALSE(button.released());
  VERIFY_AND_CLEAR();
}

TEST_F(YablTest, singleTapEvent) {
  SET_PIN_LEVEL_AT_MILLIS(LOW, 100);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(PRESS));
  EXPECT_FALSE(button.triggered(RELEASE | SHORT_RELEASE | SINGLE_TAP | DOUBLE_TAP | HOLD |
                                LONG_RELEASE | USER_EVENT));
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 200);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(RELEASE | SHORT_RELEASE));
  EXPECT_FALSE(button.triggered(PRESS | SINGLE_TAP | DOUBLE_TAP | HOLD | LONG_RELEASE |
                                USER_EVENT));
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 300);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.triggered(ALL_EVENTS));
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(HIGH, 400);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(SINGLE_TAP));
  EXPECT_FALSE(button.triggered(PRESS | RELEASE | SHORT_RELEASE | DOUBLE_TAP | HOLD |
                                LONG_RELEASE | USER_EVENT));
  VERIFY_AND_CLEAR();
}


TEST_F(YablTest, doubleTapEvent) {
  SET_PIN_LEVEL_AT_MILLIS(LOW, 100);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(PRESS));
  EXPECT_FALSE(button.triggered(RELEASE | SHORT_RELEASE | SINGLE_TAP | DOUBLE_TAP | HOLD |
                                LONG_RELEASE | USER_EVENT));
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 200);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(RELEASE | SHORT_RELEASE));
  EXPECT_FALSE(button.triggered(PRESS | SINGLE_TAP | DOUBLE_TAP | HOLD | LONG_RELEASE |
                                USER_EVENT));
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(LOW, 300);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(PRESS));
  EXPECT_TRUE(button.triggered(DOUBLE_TAP));
  EXPECT_FALSE(button.triggered(RELEASE | SHORT_RELEASE | SINGLE_TAP | HOLD | LONG_RELEASE |
                                USER_EVENT));
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(HIGH, 400);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(RELEASE | SHORT_RELEASE));
  EXPECT_FALSE(button.triggered(PRESS | SINGLE_TAP | DOUBLE_TAP | HOLD | LONG_RELEASE |
                                USER_EVENT));
  VERIFY_AND_CLEAR();
}


TEST_F(YablTest, holdEvent) {
  SET_PIN_LEVEL_AT_MILLIS(LOW, 100);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(PRESS));
  EXPECT_FALSE(button.triggered(RELEASE | SHORT_RELEASE | SINGLE_TAP | DOUBLE_TAP | HOLD |
                                LONG_RELEASE | USER_EVENT));
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(LOW, 300);
  EXPECT_FALSE(button.update());
  EXPECT_FALSE(button.triggered(ALL_EVENTS));
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(LOW, 500);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(HOLD));
  EXPECT_FALSE(button.triggered(PRESS | RELEASE | SHORT_RELEASE | SINGLE_TAP | DOUBLE_TAP |
                                LONG_RELEASE | USER_EVENT));
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(HIGH, 700);
  EXPECT_TRUE(button.update());
  EXPECT_TRUE(button.triggered(RELEASE));
  EXPECT_TRUE(button.triggered(LONG_RELEASE));
  EXPECT_FALSE(button.triggered(PRESS | SHORT_RELEASE | SINGLE_TAP | DOUBLE_TAP | HOLD |
                                USER_EVENT));
  VERIFY_AND_CLEAR();
}

TEST_F(YablTest, callbacks) {
  InSequence dummy;
  
  button.callback(callbackWithEventInfo);
  
  SET_PIN_LEVEL_AT_MILLIS(LOW, 100);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), PRESS));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 200);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), RELEASE));
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), SHORT_RELEASE));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 400);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), SINGLE_TAP));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(LOW, 500);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), PRESS));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(HIGH, 600);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), RELEASE));
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), SHORT_RELEASE));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(LOW, 700);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), PRESS));
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), DOUBLE_TAP));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 800);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), RELEASE));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();

  SET_PIN_LEVEL_AT_MILLIS(LOW, 900);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), PRESS));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(LOW, 1300);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), HOLD));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
  
  SET_PIN_LEVEL_AT_MILLIS(HIGH, 1400);
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), RELEASE));
  EXPECT_CALL(callback, callbackWithEventInfo(Ref(button), LONG_RELEASE));
  EXPECT_TRUE(button.update());
  VERIFY_AND_CLEAR();
}
