#ifndef INFRARED_SENSOR
#define INFRARED_SENSOR

// START:main
#include <stdint.h>
#include "ring_buffer.h"

using namespace arduino::util;

namespace arduino {
  namespace sensors {
    class InfraredSensor {
      private:
        uint8_t              _pin;
        RingBuffer<uint16_t> _buffer;
      public:
       static const float SUPPLY_VOLTAGE = 4.7;
       static const float VOLTS_PER_CM   = 27.0;

        InfraredSensor(const uint8_t pin);

        void update(void);
        float getDistance(void) const;
    };
  }
}
// END:main

#endif

// vim:ft=arduino
