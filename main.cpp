#include <algorithm>
#include <exception>
#include <iostream>
#include <utility>

#include <lame.h>

#include "monitor.h"
#include "thread.h"
#include "wav_files.h"

using namespace wav2mp3;

namespace wav2mp3
{
  std::string normalize_path(std::string path)
  {
    if (path.empty())
      return path;

    std::replace(path.begin(), path.end(), '\\', '/');

    if (path.back() == '/')
      path.pop_back();

    return path;
  }


void convert(std::string const& filename)
{
  lame_global_flags* encoder = lame_init();
  lame_set_quality(encoder, 5);
  lame_init_params(encoder);
  lame_close(encoder);
}

void process(std::vector<std::string> const& files)
{
  monitor<std::reference_wrapper<std::ostream>> synchronized_cout{ std::cout };
  monitor<size_t> atomic_counter{ 0 };

  size_t const hardware_concurrency = thread::hardware_concurrency();
  //std::cout << "Hardware concurrency: " << hardware_concurrency << std::endl;

  size_t const threadCount = std::min<size_t>(hardware_concurrency, files.size());
  //std::cout << "Thread count: " << threadCount << std::endl;

  std::vector<thread> threads;
  threads.reserve(threadCount);

  for (size_t t = 0; t < threadCount; ++t)
    threads.emplace_back(
      [&,t]()
  {
    while (true)
    {
      size_t const position = atomic_counter([](size_t& counter)
      {
        return counter++;
      });
      if (position >= files.size())
        break;

      synchronized_cout([&](std::ostream& str)
      {
        str << "Converting " << files[position];
        //str << " in thread  " << t;
        str << std::endl;
      });

      convert(files[position]);

    }
  });
}
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: <" << argv[0] << "> path_to_wav_collection" << std::endl;
    return 1;
  }

  try
  {
    auto const path = normalize_path(argv[1]);
    auto const files = wav_files(path.c_str());
    process(files);
  }
  catch (std::exception& e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
