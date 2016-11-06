#include "mutex.h"

#include <system_error>

#ifdef _WIN32
#include <time.h>
#define HAVE_STRUCT_TIMESPEC 1
#endif
#include <pthread.h>

namespace wav2mp3 {
class mutex::impl
{
public:
  impl()
  {
    if (auto const error_code = pthread_mutex_init(&mutex_, nullptr))
      throw new std::system_error(error_code, std::system_category());
  }
  ~impl() { pthread_mutex_destroy(&mutex_); }

  void lock() { pthread_mutex_lock(&mutex_); }
  void unlock() { pthread_mutex_unlock(&mutex_); }

private:
  pthread_mutex_t mutex_;
};

mutex::mutex()
  : impl_{ std::make_unique<impl>() }
{
}

mutex::~mutex() = default;

void
mutex::lock()
{
  impl_->lock();
}

void
mutex::unlock()
{
  impl_->unlock();
}
}
