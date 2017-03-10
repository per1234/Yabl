#pragma once

namespace Arduino {

unsigned long _millis = 0;

unsigned long millis() {
  return _millis;
}

} // namespace Arduino

using namespace Arduino;
