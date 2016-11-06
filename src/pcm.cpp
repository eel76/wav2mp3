#include "pcm.h"
#include "wave_format_exception.h"
#include "wave_header.h"

#include <cassert>
#include <istream>

namespace wav2mp3 {

std::istream&
operator>>(std::istream& istr, pcm& data)
{
  wave_header header;
  istr >> header;

  if (header.audio_format != wave_header::PCM)
    throw wave_format_exception("Unsupported format");

  if (header.bits_per_sample != 16)
    throw wave_format_exception("Unsupported format");

  if (header.number_of_channels != 1 && header.number_of_channels != 2)
    throw wave_format_exception("Unsupported format");

  auto const bytes_per_sample = header.bits_per_sample / 8;
  auto const number_of_samples = header.data_size / bytes_per_sample;

  assert(header.data_size == sizeof(pcm::sample) * number_of_samples);

  std::vector<pcm::sample> samples;
  samples.resize(number_of_samples);

  if (!istr.read(reinterpret_cast<char*>(samples.data()), header.data_size))
    throw wave_format_exception("Unexpected end of file");

  swap(data.samples_, samples);
  data.samples_per_second_ =
    static_cast<pcm::samplerate>(header.samples_per_second);
  data.number_of_channels_ =
    static_cast<pcm::channels>(header.number_of_channels);

  return istr;
}
}
