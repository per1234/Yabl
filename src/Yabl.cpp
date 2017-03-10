#include "Yabl.h"
#include <Arduino.h>

namespace Yabl {

bool Button::update() {
  unsigned long elasped = millis() - previous_millis;

  if (!gestureStarted()) {
    _suppressEvents = 0;
  }
  
  if (_reset) {
    reset();
  }
  
  Bounce::update();

  clearEvents();

  if (pressed()) {
    triggerEvent(PRESS);
      
    if (gestureIncludes(RELEASE) && elasped < _doubleTapInterval) {
      triggerEvent(DOUBLE_TAP);
    }
  }
  else if (released()) {
    if (!gestureStarted()) {
      return false;
    }
    triggerEvent(RELEASE);
      
    if (gestureIncludes(HOLD)) {
      triggerEvent(LONG_RELEASE);
      _reset = true;
    }
    else if (gestureIncludes(DOUBLE_TAP)) {
      _reset = true;
    }
    else {
      triggerEvent(SHORT_RELEASE);
    }
  }
  else if (down()) {
    if (gestureStarted() && !gestureIncludes(HOLD) && !gestureIncludes(RELEASE) && elasped >= _holdDuration) {
      triggerEvent(HOLD);
    }
  }
  else {
    if (gestureIncludes(PRESS) && elasped >= _doubleTapInterval) {
      triggerEvent(SINGLE_TAP);
      _reset = true;
    }
  }
  
  return activity();
}

void Button::reset()
{
  _currentEvents = 0;
  _gestureEvents = 0;
  _suppressEvents = 0;
  _reset = false;
}

void Button::wakeup() {
  previous_millis = millis();
}

void Button::triggerEvent(Event event) {
  if (_suppressEvents & event) { 
    return;
  }
  
  _currentEvents |= event;
  _gestureEvents |= event;

  switch (_callbacks[event].type) {
    case Callback::SIMPLE: (_callbacks[event].callback.simple)(); break;
    case Callback::WITH_BUTTON_AND_EVENT: (_callbacks[event].callback.withButtonAndEvent)(*this, event); break;
    case Callback::NONE: break;
  }
}

void Button::callback(CallbackSimple callback, Event forEvents) {
  for (int i = 0; i < EVENT_COUNT; ++i) {
    if (forEvents & (1 << i)) {
      _callbacks[i].type = Callback::SIMPLE;
      _callbacks[i].callback.simple = callback;
    }
  }
}

void Button::callback(CallbackWithButtonAndEvent callback, Event forEvents) {
  for (int i = 0; i < EVENT_COUNT; ++i) {
    if (forEvents & (1 << i)) {
      _callbacks[i].type = Callback::WITH_BUTTON_AND_EVENT;
      _callbacks[i].callback.withButtonAndEvent = callback;
    }
  }
}

} // namespace Yabl

