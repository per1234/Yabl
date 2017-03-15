/*
 * =============================================================================
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 * -----------------------------------------------------------------------------
 * This example demonstrates how to use one callback for multiple buttons and
 * events. The sketch requires two buttons be connected to pins `BUTTON_A_PIN`
 * and `BUTTON_B_PIN` defined below. The pins are configured to pull-up meaning
 * the buttons must also be connected to ground. Open the Serial Monitor and
 * press the buttons to see events triggered on both buttons.
 */

#include <Yabl.h>

#define BUTTON_A_PIN 5 // MCU pin connected to button A
#define BUTTON_B_PIN 6 // MCU pin connected to button B

Button buttonA;
Button buttonB;

void setup() {
  Serial.begin(9600); // initialize the serial port
  buttonA.attach(BUTTON_A_PIN, INPUT_PULLUP); // button also connected to GND
  buttonB.attach(BUTTON_B_PIN, INPUT_PULLUP); // button also connected to GND
  buttonA.callback(onButtonEvent); // callback for all button events
  buttonB.callback(onButtonEvent); // callback for all button events
}

void onButtonEvent(const EventInfo& info) {
  Serial.print(info.button == buttonA ?  "Button A: " : "Button B: ");
  switch (info.event) {
    case PRESS: Serial.print("PRESS\n"); break;
    case RELEASE: Serial.print("RELEASE\n"); break;
    case SHORT_RELEASE: Serial.print("SHORT_RELEASE\n"); break;
    case SINGLE_TAP: Serial.print("SINGLE_TAP\n"); break;
    case DOUBLE_TAP: Serial.print("DOUBLE_TAP\n"); break;
    case HOLD: Serial.print("HOLD\n"); break;
    case LONG_RELEASE: Serial.print("LONG_RELEASE\n"); break;
    case USER_EVENT: Serial.print("EXCLUSIVE_PRESS\n"); break;
    default: break;
  }
}

void loop() {
  /* update both button states - callbacks are called inside `update()` */
  buttonA.update();
  buttonB.update();
}
