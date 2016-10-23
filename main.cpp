#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <system_error>
#include <utility>

#include <lame.h>

#include "monitor.h"
#include "pcm.h"
#include "thread.h"
#include "wav_files.h"
#include "lame_encoder_exception.h"
#include "wave_format_exception.h"

using namespace wav2mp3;

namespace wav2mp3 {

class lame_encoder
{
public:
  lame_encoder()
    : global_flags_{ lame_init() }
  {
    if (!global_flags_)
      throw lame_encoder_exception{ "Unable to init lame encoder" };
  }
  lame_encoder(lame_encoder const&) = delete;
  lame_encoder& operator=(lame_encoder const&) = delete;
  ~lame_encoder()
  {
    lame_close(global_flags_);
  }

  operator lame_global_flags*() const
  {
    return global_flags_;
  }

private:
  lame_global_flags* global_flags_;
};

  std::vector<unsigned char> encode(pcm const& source)
  {
    lame_encoder encoder;

    if (lame_set_in_samplerate(encoder, source.samples_per_second()) != 0)
      throw lame_encoder_exception{ "Unable to init encoder samplerate" };

    if (lame_set_num_channels(encoder, source.channels()) != 0)
      throw lame_encoder_exception{ "Unable to init encoder channels" };

    if (lame_set_quality(encoder, 5) != 0)
      throw lame_encoder_exception{ "Unable to init encoder quality" };

    if (lame_init_params(encoder) != 0)
      throw lame_encoder_exception{ "Unable to init encoder quality" };

    std::vector<pcm::sample> samples = source.samples();

    std::vector<unsigned char> buffer;
    buffer.resize(source.samples().size() * 5 / 4 + 7200);

    int mp3size =
      lame_encode_buffer_interleaved(
        encoder,
        &(samples.data()->left),
        static_cast<int>(samples.size()),
        buffer.data(),
        static_cast<int>(buffer.size()));

    if (mp3size < 0)
      return{};

    std::vector<unsigned char> frames;
    frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

    mp3size = lame_encode_flush(encoder, buffer.data(),
      static_cast<int>(buffer.size()));

    if (mp3size > 0)
      frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

    return frames;
  }

class mp3
{
public:
  explicit mp3(pcm const& source)
    : frames_{ encode(source) }
  {
  }
  ~mp3() = default;

  friend std::ostream& operator << (std::ostream& ostr, mp3 const& data)
  {
    return ostr.write(reinterpret_cast<char const*> (data.frames_.data()), data.frames_.size());
  }

  std::vector<unsigned char> const& frames() const
  {
    return frames_;
  }

private:
  std::vector<unsigned char> frames_;
};

void convert(path filename)
{
  pcm input;
  std::ifstream{ filename, std::ifstream::binary } >> input;
  std::ofstream{ filename.replace_extension(".mp3"), std::ofstream::binary } << mp3{ input };

}
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
      while (true) {
        size_t const i = atomic([](size_t& value) { return value++; });
        if (i >= collection.size())
          break;

        try {
          synchronized_cout([&](std::ostream& str) {
            str << t << ": processing " << collection[i] << std::endl;
          });

          convert(collection[i]);

          synchronized_cout([&](std::ostream& str) {
            str << t << ": converted  " << collection[i] << std::endl;
          });
        }
        catch (wave_format_exception& e) {
          synchronized_cout([&](std::ostream& str) {
            str << t << ": skipped    " << collection[i] << ": " << e.what() << std::endl;
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
