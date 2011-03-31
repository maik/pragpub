// START:main
#include <WProgram.h>
#include "pdc.h"

namespace arduino {
  void ParkDistanceControl::check(void) {
    _ir_sensor.update();
    const float distance =
      _ir_sensor.getDistance() - _mounting_gap;
    if (distance <= MIN_DISTANCE) {
      Serial.println("Too close!");
      _speaker.beep();
    } else if (distance >= MAX_DISTANCE) {
      Serial.println("OK.");
    } else {
      Serial.print(distance);
      Serial.println(" cm");
    }
  }
}
// END:main

// vim:ft=arduino
