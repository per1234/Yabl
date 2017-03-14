/*
 * =============================================================================
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 * -----------------------------------------------------------------------------
 */

#include "Yabl.h"
#include <Arduino.h>

namespace Yabl {

bool EventInfo::operator==(const EventInfo& other) const {
  return other.button == button && other.event == event;
}
  

bool Button::update() {
  unsigned long elasped = millis() - previousMillis();

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
  
  Callback* cb = callback(event);
  switch (cb->type) {
    case Callback::SIMPLE: (cb->callback.simple)(); break;
    case Callback::WITH_EVENT_INFO: (cb->callback.withEventInfo)(EventInfo{*this, event}); break;
    case Callback::NONE: break;
  }
}
  
Button::Callback* Button::callback(Event forEvent) {
  for (int i = 0; i < EVENT_COUNT; ++i) {
    if (forEvent & (1 << i)) {
      return _callbacks + i;
    }
  }
  
  return 0;
}

void Button::callback(CallbackSimple callback, Event forEvents) {
  for (int i = 0; i < EVENT_COUNT; ++i) {
    if (forEvents & (1 << i)) {
      _callbacks[i].type = Callback::SIMPLE;
      _callbacks[i].callback.simple = callback;
    }
  }
}

void Button::callback(CallbackWithEventInfo callback, Event forEvents) {
  for (int i = 0; i < EVENT_COUNT; ++i) {
    if (forEvents & (1 << i)) {
      _callbacks[i].type = Callback::WITH_EVENT_INFO;
      _callbacks[i].callback.withEventInfo = callback;
    }
  }
}

} // namespace Yabl

