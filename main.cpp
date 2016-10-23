#include <algorithm>
#include <fstream>
#include <iostream>
#include <system_error>
#include <utility>

#include "monitor.h"
#include "mp3.h"
#include "pcm.h"
#include "thread.h"
#include "wav_files.h"
#include "wave_format_exception.h"

// #include "lame_encoder.cpp"

using namespace wav2mp3;

namespace wav2mp3 {

void process(path filename)
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
    threads.emplace_back([&,t]() {
      while (true) {
        size_t const i = atomic([](size_t& value) { return value++; });
        if (i >= collection.size())
          break;

        try {
          synchronized_cout([&](std::ostream& str) {
            str << t << ": processing " << collection[i] << std::endl;
          });

          process(collection[i]);

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
