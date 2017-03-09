# Yabl
Yet Another Button Library (for Arduino).

This library augments the popular debounce library [Bounce2](https://github.com/thomasfredericks/Bounce2) with the ability to set callbacks or easily check for the following button events:
* Press
* Release
* Hold
* Tap
* Double-tap

## Using callbacks
This starts off the same way as setting up [Bounce2](https://github.com/thomasfredericks/Bounce2). To set up a button on pin 6 and set a callback for button press and release events:
```c++
Button button;

void setup() {
  button.attach(6);
  button.callback(onButtonPress, PRESS);
  button.callback(onButtonRelease, RELEASE);
}

void onButtonPress() {
  // ...
}

void onButtonRelease() {
  // ...
}
```
Callbacks are triggered when the button's state is updated by calling update() in the main loop:
```c++
void loop() {
  button.update();
  // ...
}
```
Callbacks can be also be added for the events `SHORT_RELEASE`, `LONG_RELEASE`, `HOLD`, `SINGLE_TAP` and `DOUBLE_TAP`. In fact, sometimes you may want to have a single callback to handle all events:
```c++
void setup() {
  button.attach(6);
  button.callback(onButtonEvent);
}

void onButtonEvent(Button& button, Event event) {
  switch (event) {
    case SINGLE_TAP: /* ... */ break;
    case DOUBLE_TAP: /* ... */ break;
    case HOLD: /* ... */ break;
  }
}
```

## Checking for events
An alternative to using callbacks is to check the button for activity after each `update()`:
```c++
void setup() {
  button.attach(6);
}

void update() {
  if (button.update()) {
    // an event was triggered
    if (button.triggered(SINGLE_TAP))
      // ...
    else if (button.triggered(DOUBLE_TAP))
      // ...
    else if (button.triggered(HOLD))
      // ...
  }
}
```
