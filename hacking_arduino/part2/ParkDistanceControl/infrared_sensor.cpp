// START:main
#include <WProgram.h>
#include <stdint.h>
#include "infrared_sensor.h"

namespace arduino {
  namespace sensors {
    InfraredSensor::InfraredSensor(const uint8_t pin) 
      : _pin(pin) 
    {
      for (uint16_t i = 0; i < _buffer.getBufferSize(); i++)
        update();
    }

    float InfraredSensor::getDistance(void) const {
      const float voltage = 
        _buffer.getAverageValue() * SUPPLY_VOLTAGE / 1024.0;
      return VOLTS_PER_CM / voltage;
    }

    void InfraredSensor::update(void) {
      _buffer.addValue(analogRead(_pin));
    }
  }
}
// END:main

// vim:ft=arduino
