#include <cstdint>
#include <iosfwd>

namespace wav2mp3 {

struct wave_header
{
  enum
  {
    PCM = 1
  };

  char     riff[4];
  uint32_t riff_size;
  char     wave[4];
  char     fmt[4];
  uint32_t fmt_size;
  uint16_t audio_format;
  uint16_t number_of_channels;
  uint32_t samples_per_second;
  uint32_t bytes_per_second;
  uint16_t block_alignment;
  uint16_t bits_per_sample;
  char     data[4];
  uint32_t data_size;
};

std::istream& operator>>(std::istream& istr, wave_header& ret);
}
