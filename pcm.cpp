#include "pcm.h"
#include "wave_format_exception.h"

#include <cassert>
#include <istream>

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

std::istream& operator >> (std::istream& istr, wave_header& ret)
{
  wave_header header;

  if (!istr.read(header.riff, sizeof(header)))
    throw wave_format_exception("header not found");

  if (std::string{ header.riff, sizeof(header.riff) } != "RIFF")
    throw wave_format_exception("RIFF header not found");

  if (std::string{ header.wave, sizeof(header.wave) } != "WAVE")
    throw wave_format_exception("WAVE header not found");

  if (std::string{ header.fmt, sizeof(header.fmt) } != "fmt ")
    throw wave_format_exception("FMT header not found");

  if (header.fmt_size != 16)
    throw wave_format_exception("Unexpected FMT header size");

  if (header.audio_format != wave_header::PCM)
    throw wave_format_exception("Unsupported format");

  if (header.bits_per_sample != 16)
    throw wave_format_exception("Unsupported format");

  if (header.number_of_channels != 2)
    throw wave_format_exception("Unsupported format");

  if (std::string{ header.data, sizeof(header.data) } != "data")
    throw wave_format_exception("DATA header not found");

  using namespace std;
  swap(header, ret);
  return istr;
}

std::istream& operator >> (std::istream& istr, pcm& data)
{
  wave_header header;
  istr >> header;

  uint16_t const bytes_per_sample = header.bits_per_sample / 8;
  size_t const number_of_samples = header.data_size / bytes_per_sample / header.number_of_channels;

  assert(header.data_size == sizeof(pcm::sample)* number_of_samples);

  std::vector<pcm::sample> samples;
  samples.resize(number_of_samples);

  if (!istr.read(reinterpret_cast<char*>(samples.data()), header.data_size))
    throw wave_format_exception("Unexpected end of file");

  swap(data.samples_, samples);
  data.samples_per_second_ = header.samples_per_second;

  return istr;
}
}
