#ifndef SPEAKER
#define SPEAKER

// START:main
#include <stdint.h>
#include "WProgram.h"

namespace arduino {
  namespace sensors {
		const uint16_t DEF_FREQUENCY = 1000;
		const uint16_t DEF_DURATION  = 200;

		class Speaker {
			public:
				Speaker(const uint16_t speaker_pin) :
					_speaker_pin(speaker_pin) {}

				void beep(
					const uint16_t frequency = DEF_FREQUENCY,
					const uint16_t duration  = DEF_DURATION) const
				{
					tone(_speaker_pin, frequency, duration);
				}

			private:
				uint16_t _speaker_pin;
		};
	}
}
// END:main

#endif

