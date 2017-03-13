#include <Yabl.h>

#define BUTTON_PIN 6 // set this to the pin the button is connected to on your board

Button button;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  button.attach(BUTTON_PIN, INPUT_PULLUP);
}

/*
 * pressed() and released() only return `true` after the first update() following
 * a button press/release.
 */
void loop() {
  button.update();
  if (button.pressed())
    digitalWrite(LED_BUILTIN, HIGH);
  else if (button.released())
    digitalWrite(LED_BUILTIN, LOW);
}

