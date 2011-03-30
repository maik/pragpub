// START:main
#include <WProgram.h>
#include "park_distance_control.h"

namespace arduino {
  ParkDistanceControl::ParkDistanceControl(
    const InfraredSensor& ir_sensor,
    const Speaker&        speaker,
    const float           mounting_gap) :
      _ir_sensor(ir_sensor),
      _speaker(speaker),
      _mounting_gap(mounting_gap)
  {
  }

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
