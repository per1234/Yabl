/*
 * =============================================================================
 * Yet Another Button Library (for Arduino)
 * https://github.com/yergin/Yabl
 * Copyright 2017 Gino Bollaert
 * MIT License (https://opensource.org/licenses/mit-license.php)
 * -----------------------------------------------------------------------------
 */

#pragma once

#include <Bounce2.h>
#include <cinttypes>

/*
 * -----------------------------------------------------------------------------
 * The Yabl namespace prevents name colisions with other possible Button and
 * Event classed.
 */
namespace Yabl {

/*
 * -----------------------------------------------------------------------------
 * The following `Event` type and constants describe events the `Button` class
 * detects. These events can often be combined using a bit-wise OR when passed
 * as parameters to `Button` class methods. See `Button` class method comments.
 */
typedef uint16_t Event;

/* pressed */
static constexpr Event PRESS = 0x01;
/* released */
static constexpr Event RELEASE = 0x02;
/* released before `holdDuration` */
static constexpr Event SHORT_RELEASE = 0x04;
/* relased (short) and not pressed again before `doubleTapInterval` */
static constexpr Event SINGLE_TAP = 0x08;
/* elased (short) and pressed again within `doubleTapInterval` */
static constexpr Event DOUBLE_TAP = 0x10;
/* pressed and held for `holdDuration` */
static constexpr Event HOLD = 0x20;           
/* relased after being held for at least `holdDuration` */
static constexpr Event LONG_RELEASE = 0x40;
/* additional events for subclasses can start here */
static constexpr Event USER_EVENT = 0x100;
/* mask for all events */
static constexpr Event ALL_EVENTS = 0xFFFF;

/*
 * -----------------------------------------------------------------------------
 * The `EventInfo` struct is passed as a parameter to event callbacks specifying
 * the button and type of event triggered.
 */
  
class Button;

struct EventInfo {
  Button& button;
  Event event;
  bool operator==(const EventInfo& other) const;
};

/*
 * =============================================================================
 * The `Button` class is the main class and represents a physical button
 * attached to the board. It subclasses Bounce and therefore all public Bounce
 * methods such as `attach(...)` and `interval()` are available to the user of
 * this class. `read()`, `fell()` and `rose()` are also available but the
 * methods `down()`, `pressed()` and `released()` are preferred as they are more
 * meaningful when describing a button state or event.
 * 
 * Example of use:
 *
 *   Button button;
 *
 *   // Specify the pin the button is attached to using the `Bounce` method
 *   //`attach(...):
 *   void setup() {
 *     button.attach(6, INPUT_PULLUP); // button attached to pin 6 and ground
 *   }
 *
 *   // respond to button events in the main loop
 *   void loop() {
 *     button.update(); // update button state 
 *     if (button.pressed()) {
 *       // respond to button press here...
 *     }
 *   }
 * -----------------------------------------------------------------------------
 */
  
class Button : public Bounce
{
public:
  /*
   * There are two possibile function signitures for callbacks. See comments for
   * both `callback(...)` methods below.
   */
  typedef void (*CallbackSimple)();
  typedef void (*CallbackWithEventInfo)(const EventInfo&);
  
  Button() {}

  bool update();
  void reset();
  void sleep() { reset(); }
  void wakeup();
  
  bool down() { return read() != _inverted; }
  bool pressed() { return _inverted ? fell() : rose(); }
  bool released() { return _inverted ? rose() : fell(); }
  bool activity() const { return _currentEvents != 0; }
  bool triggered(Event event) const { return _currentEvents & event; }
  bool gestureStarted() const { return _gestureEvents != 0; }
  bool gestureIncludes(Event event) const { return _gestureEvents & event; }
  void suppressOnce(Event event) { _suppressEvents |= event; }
  
  void holdDuration(unsigned int ms) { _holdDuration = ms; }
  unsigned int holdDuration() const { return _holdDuration; }
  void doubleTapInterval(unsigned int ms) { _doubleTapInterval = ms; }
  unsigned int doubleTapInterval() const { return _doubleTapInterval; }
  void inverted(bool inverted) { _inverted = inverted; }
  bool inverted() const { return _inverted; }
  
  /*
   * Callbacks with signiture `CallbackSimple` have no arguments and are
   * typically used when there is one button and seperate callbacks per-event.
   * Multiple events can be linked to the same callback by combining events
   * using the bit-wise OR operator. For example:
   *
   *   void setup() {
   *     ...
   *     button.callback(onButtonTap, SINGLE_TAP | HOLD);
   *     button.callback(onButtonDoubleTap, DOUBLE_TAP);
   *   }
   *   
   *   void onButtonTap() {
   *     // respond to button tap here...
   *   }
   *
   *   void onButtonDoubleTap() {
   *     // respond to button double-tap here...
   *   }
   *   
   *   void loop() {
   *     button.update();
   *     ...
   *   }
   */
  void callback(CallbackSimple callback, Event forEvents);
  void callback(CallbackWithEventInfo callback, Event forEvents = ALL_EVENTS);

  bool operator==(const Button& other) const { return this == &other; }

protected:
  void triggerEvent(Event event);
  unsigned long previousMillis() const { return previous_millis; }
  
private:
  struct Callback {
    Callback() : type(NONE) {}
    enum {
      NONE = 0,
      SIMPLE,
      WITH_EVENT_INFO
    } type;
    union {
      CallbackSimple simple;
      CallbackWithEventInfo withEventInfo;
    } callback;
  };
  
  void clearEvents() { _currentEvents = 0; }
  Callback* callback(Event forEvent);

  static constexpr int EVENT_COUNT = sizeof(Event) * 8;

  bool _inverted = true;
  unsigned int _holdDuration = 400;
  unsigned int _doubleTapInterval = 150;
  bool _reset = false;
  Event _currentEvents = 0;
  Event _gestureEvents = 0;
  Event _suppressEvents = 0;
  Callback _callbacks[EVENT_COUNT];
};


} // namespace Yabl

using namespace Yabl;
