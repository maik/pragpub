#ifndef SPEAKER
#define SPEAKER

// START:main
#include <stdint.h>
#include "WProgram.h"

namespace arduino {
  namespace actuators {
    class Speaker {
      public:
        static const uint16_t DEF_FREQUENCY = 1000;
        static const uint32_t DEF_DURATION  = 200;

        Speaker(const uint8_t speaker_pin) : // <label id="code.speaker.constructor"/>
          _speaker_pin(speaker_pin) {} // <label id="code.speaker.mil"/>

        void beep(
          const uint16_t frequency = DEF_FREQUENCY,
          const uint32_t duration  = DEF_DURATION) const // <label id="code.speaker.const_method"/>
        {
          tone(_speaker_pin, frequency, duration);
        }

      private:
        uint8_t _speaker_pin;
    };
  }
}
// END:main

#endif

