#ifndef INFRARED_SENSOR
#define INFRARED_SENSOR

// START:main
#include <stdint.h>
#include "ring_buffer.h"

using namespace arduino::util;

namespace arduino {
  namespace sensors {
    const float SUPPLY_VOLTAGE = 4.7;
    const float VOLTS_PER_CM   = 27.0;

    class InfraredSensor {
      public:
        InfraredSensor(const uint16_t pin);

        float getDistance(void) const;
        void update(void);

      private:
        RingBuffer<uint16_t> _buffer;
        uint16_t             _pin;
    };
  }
}
// END:main

#endif

// vim:ft=arduino
