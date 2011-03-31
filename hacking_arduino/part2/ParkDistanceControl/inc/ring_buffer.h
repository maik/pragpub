#ifndef RING_BUFFER
#define RING_BUFFER

#include <stdlib.h>
#include <math.h>

// START:main
namespace arduino {
  namespace util {
    template<typename T> // <label id="code.rbuffer.template"/>
    class RingBuffer {
      private:
        T*       _samples; // <label id="code.rbuffer.buffer"/>
        uint16_t _sample_pos;
        uint16_t _buffer_size;
      public:
        static const uint16_t DEF_SIZE = 16;

        RingBuffer(const uint16_t buffer_size = DEF_SIZE) { // <label id="code.rbuffer.constructor"/>
          _sample_pos = 0;
          _buffer_size = buffer_size != 0 ? buffer_size : DEF_SIZE;
          _samples = static_cast<T*>(
            malloc(sizeof(T) * _buffer_size)
          );
        }

        RingBuffer(const RingBuffer& rhs) { // <label id="code.rbuffer.copyconstructor"/>
          *this = rhs;
        }

        RingBuffer& operator=(const RingBuffer& rhs) { // <label id="code.rbuffer.assignment"/>
          if (this != &rhs) {
            _sample_pos = rhs._sample_pos;
            _buffer_size = rhs._buffer_size;
            _samples = static_cast<T*>(
              malloc(sizeof(T) * _buffer_size)
            );
            for (uint16_t i = 0; i < _buffer_size; i++)
              _samples[i] = rhs._samples[i];
          }
          return *this;
        }

        ~RingBuffer() { // <label id="code.rbuffer.destructor"/>
          free((void*)_samples);
        }

        void addValue(const T value) {
          _samples[_sample_pos] = value;
          _sample_pos = (_sample_pos + 1) % _buffer_size; // <label id="code.rbuffer.modulus"/>
        }

        T getAverageValue() const {
          float sum = 0.0;
          for (uint16_t i = 0; i < _buffer_size; i++)
            sum += _samples[i];
          return round(sum / _buffer_size);
        }

        uint16_t getBufferSize() const {
          return _buffer_size;
        }
    };
  }
}
// END:main

#endif

// vim:ft=arduino
