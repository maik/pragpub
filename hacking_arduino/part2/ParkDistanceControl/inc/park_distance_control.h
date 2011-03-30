#ifndef PARK_DISTANCE_CONTROL
#define PARK_DISTANCE_CONTROL

// START:main
#include <stdint.h>
#include "infrared_sensor.h"
#include "speaker.h"

using namespace arduino::sensors;

namespace arduino {
  const float MIN_DISTANCE = 8.0;
  const float MAX_DISTANCE = 80.0;

  class ParkDistanceControl {
    public:
      ParkDistanceControl(
        const InfraredSensor& ir_sensor,
        const Speaker&        speaker,
        const float           mounting_gap = 0.0);

      void check(void);

    private:
      InfraredSensor _ir_sensor;
      Speaker        _speaker;
      float          _mounting_gap;
  };
}
// END:main

#endif

// vim:ft=arduino
