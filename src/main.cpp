#include "mp3.h"
#include "pcm.h"
#include "synchronize.h"
#include "thread.h"
#include "wave_files.h"
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

  mp3 output{ input };
  ofstream{ filename.replace_extension(".mp3"), ofstream::binary } << output;
}

void
process(vector<path> const& collection)
{
  auto atomic = synchronize([value{ 0 }]() mutable { return value++; });
  auto sync = make_synchronize();

  size_t const hardware_concurrency = thread::hardware_concurrency();
  size_t const thread_count = min(hardware_concurrency, collection.size());

  vector<thread> threads;
  threads.reserve(thread_count);

  for (size_t t = 0; t < thread_count; ++t)
    threads.emplace_back([&, t] {
      while (true) {
        size_t const i = atomic();
        if (i >= collection.size())
          break;

        try {
          sync([&] {
            cout << i << "> processing " << collection[i] << " in thread " << t
                 << '\n';
          });

          process(collection[i]);

          sync([&] { cout << i << "> done" << '\n'; });
        } catch (wave_format_exception& e) {
          sync([&] { cout << i << "> failed: " << e.what() << '\n'; });
        }
      }
    });
}
}

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Usage: " << path{ argv[0] }.filename()
         << " <path-to-wav-collection>" << '\n';
    return 1;
  }

  try {
    process(wave_files(path{ argv[1] }));
  } catch (system_error const& e) {
    cout << "System error: " << e.what() << '\n';
    return e.code().value();
  } catch (exception const& e) {
    cout << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
