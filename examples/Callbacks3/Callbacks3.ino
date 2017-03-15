/*
 * =============================================================================
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 * -----------------------------------------------------------------------------
 * This example demonstrates callback registration behaviors. The
 * sketch requires a button be connected to pin `BUTTON_PIN` defined below.
 * The pin is configured to pull-up meaning the button must also be connected to
 * ground.
 */

#include <Yabl.h>

#define BUTTON_PIN 5 // MCU pin connected to button

Button button;
bool buttonFlash = false;

void setup() {
  Serial.begin(9600); // initialize Serial
  button.attach(BUTTON_PIN, INPUT_PULLUP); // button also connected to GND
  /*
   * There can only be one callback per event type. Each subsequent callback
   * registration replaces previous callbacks for the specified events. In the
   * code below, the first callback is registered for all events but these are
   * subsequntly replaced until it's only registered for `SHORT_RELEASE`.
   */
  button.callback(onOtherEvent);
  button.callback(onGestureEvent, PRESS | SINGLE_TAP | DOUBLE_TAP | HOLD);
  button.callback(onButtonPress, PRESS); // this overrides PRESS above
  button.callback(onButtonRelease, RELEASE);
  button.noCallback(LONG_RELEASE);
}

void onOtherEvent(const EventInfo& info) {
  if (info.event == SHORT_RELEASE)
    printEvent(button.eventName(info.event));
  else
    Serial.println("This should never happen!"); // All other events have other
                                                 // callbacks
}

void onButtonPress() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void onButtonRelease() {
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  button.update();
  if (button.triggered(LONG_RELEASE)) {
    digitalWrite(LED_BUILTIN, millis() % 50 < 25); // Flash every 50ms
  }
}
