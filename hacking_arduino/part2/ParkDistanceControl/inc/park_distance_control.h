#ifndef PARK_DISTANCE_CONTROL
#define PARK_DISTANCE_CONTROL

// START:main
#include <stdint.h>
#include "infrared_sensor.h"

using namespace arduino::sensors;

namespace arduino {
  const uint16_t TONE_FREQUENCY = 1000;
  const uint16_t TONE_DURATION  = 200;

  class ParkDistanceControl {
    public:
      ParkDistanceControl(
        const InfraredSensor& ir_sensor,
        const uint16_t        speaker_pin,
        const float           mounting_gap = 0.0);

      void check(void);

    private:
      InfraredSensor _ir_sensor;
      uint16_t       _speaker_pin;
      float          _mounting_gap;
  };
}
// END:main

#endif

// vim:ft=arduino
