#include "thread.h"

#include <system_error>

#ifdef _WIN32
# include <windows.h>
# define HAVE_STRUCT_TIMESPEC 1
#else
# include <unistd.h>
#endif
#include <pthread.h>

namespace wav2mp3
{
class thread::impl
{
  std::function<void()> f_;
  pthread_t mHandle;

  static void* start(void* arg)
  {
    impl* thread = reinterpret_cast<impl*> (arg);
    thread->f_();

    return nullptr;
  }

public:
  impl(std::function<void()> const& f)
    : f_{f}
  {
    int const errorCode = pthread_create(&mHandle, nullptr, &start, this);

    if (errorCode)
      throw new std::system_error(errorCode, std::system_category());
  }
  ~impl()
  {
    pthread_join(mHandle, nullptr);
  }
};

thread::thread(std::function<void()> const& f)
  : impl_{ std::make_unique<impl>(f) }
{
}
thread::thread(thread&&) = default;
thread& thread::operator=(thread&&) = default;
thread::~thread() = default;

unsigned thread::hardware_concurrency()
{
#ifdef _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
#else
  return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
}
