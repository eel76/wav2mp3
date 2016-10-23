#include "monitor.h"
#include "mp3.h"
#include "pcm.h"
#include "thread.h"
#include "wav_files.h"
#include "wave_format_exception.h"

#include <fstream>
#include <iostream>
#include <system_error>

using namespace std;
using namespace wav2mp3;

namespace {
void
process(path filename)
{
  pcm input;
  ifstream{ filename, ifstream::binary } >> input;
  ofstream{ filename.replace_extension(".mp3"), ofstream::binary }
    << mp3{ input };
}
void
process(vector<path> const& collection)
{
  monitor<reference_wrapper<ostream>> synchronized_cout{ cout };
  monitor<size_t> atomic{ 0 };

  size_t const hardware_concurrency = thread::hardware_concurrency();
  size_t const thread_count = min(hardware_concurrency, collection.size());

  vector<thread> threads;
  threads.reserve(thread_count);

  for (size_t t = 0; t < thread_count; ++t)
    threads.emplace_back([&, t]() {
      while (true) {
        size_t const i = atomic([](size_t& value) { return value++; });
        if (i >= collection.size())
          break;

        try {
          synchronized_cout([&](ostream& str) {
            str << t << ": processing " << collection[i] << endl;
          });

          process(collection[i]);

          synchronized_cout([&](ostream& str) {
            str << t << ": converted  " << collection[i] << endl;
          });
        } catch (wave_format_exception& e) {
          synchronized_cout([&](ostream& str) {
            str << t << ": skipped    " << collection[i] << ": " << e.what()
                << endl;
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
    cout << "Usage: <" << path{ argv[0] }.filename()
         << "> path_to_wav_collection" << endl;
    return 1;
  }

  try {
    process(wav_files(path{ argv[1] }));
  } catch (system_error& e) {
    cout << "System error: " << e.what() << endl;
    return e.code().value();
  } catch (exception& e) {
    cout << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}
