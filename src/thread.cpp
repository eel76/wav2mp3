#include "thread.h"

#include <system_error>

#ifdef _WIN32
#include <windows.h>
#define HAVE_STRUCT_TIMESPEC 1
#else
#include <unistd.h>
#endif
#include <pthread.h>

using namespace std;

namespace wav2mp3 {
class thread::impl
{
public:
  impl(function<void()> const& f)
    : f_{ f }
  {
    if (auto const error_code = pthread_create(&thread_, nullptr, &start, this))
      throw new system_error(error_code, system_category());
  }
  ~impl() { pthread_join(thread_, nullptr); }

private:
  function<void()> f_;
  pthread_t        thread_;

  static void* start(void* arg)
  {
    auto const thread = reinterpret_cast<impl*>(arg);
    thread->f_();

    return nullptr;
  }
};

thread::thread(function<void()> const& f)
  : impl_{ make_unique<impl>(f) }
{
}

thread::thread(thread&&) = default;

thread& thread::operator=(thread&&) = default;

thread::~thread() = default;

unsigned
thread::hardware_concurrency()
{
#ifdef _WIN32
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwNumberOfProcessors;
#else
  return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
}
