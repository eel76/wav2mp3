#include <algorithm>
#include <fstream>
#include <iostream>
#include <system_error>
#include <utility>

#include <lame.h>

#include "monitor.h"
#include "thread.h"
#include "wav_files.h"

using namespace wav2mp3;

namespace wav2mp3 {
class converter
{
public:
  converter()
    : encoder_(lame_init())
  {
    lame_set_quality(encoder_, 5);
    lame_init_params(encoder_);
  }
  converter(converter const&) = delete;
  converter& operator=(converter const&) = delete;
  ~converter() { lame_close(encoder_); }

  void process(path filename)
  {
    std::ifstream wav{ filename, std::ifstream::binary };
    int num_samples = 0;

    std::vector<unsigned char> mp3buf;
    mp3buf.resize(num_samples * 5 / 4 + 7200);

    int mp3size =
      lame_encode_buffer(encoder_, nullptr, nullptr, num_samples, mp3buf.data(),
                         static_cast<int>(mp3buf.size()));
    if (mp3size < 0)
      return;

    std::basic_ofstream<unsigned char> mp3{ filename.replace_extension(".mp3"), std::ofstream::binary };
    mp3.write(mp3buf.data(), mp3size);

    mp3size =
      lame_encode_flush(encoder_, mp3buf.data(), static_cast<int>(mp3buf.size());

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
      converter c;

      while (true) {
        size_t const i = atomic([](size_t& value) { return value++; });
        if (i >= collection.size())
          break;

        synchronized_cout([&](std::ostream& str) {
          str << "Converting " << collection[i] << std::endl;
        });

        c.process(collection[i]);
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
