/*
 * =============================================================================
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 * -----------------------------------------------------------------------------
 */

#include <Yabl.h>

#define BUTTON_PIN 6 // <- the board's pin the button is connectd to

Button button;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  button.attach(BUTTON_PIN, INPUT_PULLUP); // button also connected to GND
}

/*
 * pressed() and released() will only return `true` immediately after a state change
 * after update() is called
 */
void loop() {
  button.update(); // check for button state changes
  if (button.pressed())
    digitalWrite(LED_BUILTIN, HIGH);
  else if (button.released())
    digitalWrite(LED_BUILTIN, LOW);
}

