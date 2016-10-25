#include "thread.h"

#include <system_error>

#ifdef _WIN32
#include <windows.h>
#define HAVE_STRUCT_TIMESPEC 1
#else
#include <unistd.h>
#endif
#include <pthread.h>

namespace wav2mp3 {
class thread::impl
{
public:
  impl(std::function<void()> const& f)
    : f_{ f }
  {
    int const error_code = pthread_create(&thread_, nullptr, &start, this);

    if (error_code)
      throw new std::system_error(error_code, std::system_category());
  }
  ~impl() { pthread_join(thread_, nullptr); }

private:
  std::function<void()> f_;
  pthread_t             thread_;

  static void* start(void* arg)
  {
    impl* thread = reinterpret_cast<impl*>(arg);
    thread->f_();

    return nullptr;
  }
};

thread::thread(std::function<void()> const& f)
  : impl_{ std::make_unique<impl>(f) }
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
