#include <algorithm>
#include <iostream>
#include <system_error>
#include <utility>

#include <lame.h>

#include "monitor.h"
#include "thread.h"
#include "wav_files.h"

using namespace wav2mp3;

namespace wav2mp3 {
void
convert(path const& filename)
{
  lame_global_flags* encoder = lame_init();
  lame_set_quality(encoder, 5);
  lame_init_params(encoder);
  lame_close(encoder);
}

void
process(std::vector<path> const& collection)
{
  size_t const hardware_concurrency = thread::hardware_concurrency();
  // std::cout << "Hardware concurrency: " << hardware_concurrency << std::endl;

  size_t const thread_count =
    std::min<size_t>(hardware_concurrency, collection.size());
  // std::cout << "Thread count: " << thread_count << std::endl;

  monitor<std::reference_wrapper<std::ostream>> synchronized_out{ std::cout };
  monitor<size_t> atomic{ 0 };

  std::vector<thread> threads;
  threads.reserve(thread_count);

  for (size_t t = 0; t < thread_count; ++t)
    threads.emplace_back([&, t]() {
      while (true) {
        size_t const position = atomic([](size_t& counter) { return counter++; });
        if (position >= collection.size())
          break;

        synchronized_out([&](std::ostream& str) {
          str << "Converting " << collection[position] << std::endl;
        });

        convert(collection[position]);
      }
    });
}
}

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Usage: <" << path{ argv[0] }.filename() << "> path_to_wav_collection"
              << std::endl;
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
