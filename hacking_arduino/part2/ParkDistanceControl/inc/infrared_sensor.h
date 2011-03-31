#ifndef INFRARED_SENSOR
#define INFRARED_SENSOR

// START:main
#include <stdint.h>
#include "ring_buffer.h"

using namespace arduino::util;

namespace arduino {
  namespace sensors {
    class InfraredSensor {
      public:
       static const float SUPPLY_VOLTAGE = 4.7;
       static const float VOLTS_PER_CM   = 27.0;

        InfraredSensor(const uint8_t pin);

        float getDistance(void) const;
        void update(void);

      private:
        RingBuffer<uint16_t> _buffer;
        uint8_t              _pin;
    };
  }
}
// END:main

#endif

// vim:ft=arduino
