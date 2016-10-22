#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <system_error>
#include <utility>

#include <lame.h>

#include "monitor.h"
#include "thread.h"
#include "wav_files.h"
#include "wave_format_exception.h"

using namespace wav2mp3;

namespace wav2mp3 {

class pcm
{
public:
  struct sample
  {
    short left;
    short right;
  };
  std::vector<sample>& samples()
  {
    return samples_;
  }

  friend std::istream& operator >> (std::istream& istr, pcm& ret)
  {
    wav_header header;

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

    if (header.audio_format != wav_header::PCM)
      throw wave_format_exception("Unsupported format");

    if (header.bits_per_sample != 16)
      throw wave_format_exception("Unsupported format");

    if (header.number_of_channels != 2)
      throw wave_format_exception("Unsupported format");

    if (header.samples_per_second != 44100)
      throw wave_format_exception("Unsupported format");

    if (std::string{ header.data, sizeof(header.data) } != "data")
      throw wave_format_exception("DATA header not found");

    uint16_t const bytes_per_sample = header.bits_per_sample / 8;
    size_t const number_of_samples = header.data_size / bytes_per_sample / header.number_of_channels;

    assert(header.data_size == sizeof(sample)* number_of_samples);

    std::vector<sample> samples;
    samples.resize(number_of_samples);

    if (!istr.read(reinterpret_cast<char*>(samples.data()), header.data_size))
      throw wave_format_exception("Unexpected end of file");

    swap(ret.samples_, samples);
    return istr;
  }

private:
  struct wav_header
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

  std::vector<sample> samples_;
};

class converter
{
public:
  explicit converter(int quality)
    : encoder_(lame_init())
  {
    // int CDECL lame_set_in_samplerate(lame_global_flags *, int);
    // int CDECL lame_set_num_channels(lame_global_flags *, int);

    lame_set_quality(encoder_, quality);
    lame_init_params(encoder_);
  }
  converter(converter const&) = delete;
  converter& operator=(converter const&) = delete;
  ~converter() { lame_close(encoder_); }

  void process(path filename)
  {
    pcm input;
    std::ifstream{ filename, std::ifstream::binary } >> input;

    std::vector<unsigned char> mp3buf;
    mp3buf.resize(input.samples().size() * 5 / 4 + 7200);

    int mp3size =
      lame_encode_buffer_interleaved(
      encoder_,
      &(input.samples().data()->left),
        static_cast<int>(input.samples().size()),
      mp3buf.data(),
      static_cast<int>(mp3buf.size()));

    if (mp3size < 0)
      return;

    std::basic_ofstream<unsigned char> mp3{ filename.replace_extension(".mp3"),
                                            std::ofstream::binary };
    mp3.write(mp3buf.data(), mp3size);

    mp3size = lame_encode_flush(encoder_, mp3buf.data(),
                                static_cast<int>(mp3buf.size()));

    if (mp3size > 0)
      mp3.write(mp3buf.data(), mp3size);
  }

private:
  lame_global_flags* encoder_;
};

void
process(std::vector<path> const& collection)
{
  monitor<std::reference_wrapper<std::ostream>> synchronized_cout{ std::cout };
  monitor<size_t> atomic{ 0 };

  size_t const hardware_concurrency = thread::hardware_concurrency();
  size_t const thread_count = std::min(hardware_concurrency, collection.size());

  std::vector<thread> threads;
  threads.reserve(thread_count);

  for (size_t t = 0; t < thread_count; ++t)
    threads.emplace_back([&]() {
    converter c{5};

      while (true) {
        size_t const i = atomic([](size_t& value) { return value++; });
        if (i >= collection.size())
          break;

        try {
          c.process(collection[i]);

          synchronized_cout([&](std::ostream& str) {
            str << "Converted " << collection[i] << std::endl;
          });
        } catch (std::exception& e) {
          synchronized_cout([&](std::ostream& str) {
            str << "Skipped " << collection[i] << ": " << e.what() << std::endl;
          });
        }
      }
    });
}
}

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Usage: <" << path{ argv[0] }.filename()
              << "> path_to_wav_collection" << std::endl;
    return 1;
  }

  try {
    process(wav_files(path{ argv[1] }));
  } catch (std::system_error& e) {
    std::cout << "System error: " << e.what() << std::endl;
    return e.code().value();
  } catch (std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
