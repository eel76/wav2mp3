#include "mutex.h"

#include <system_error>

#ifdef _WIN32
# include <time.h>
# define HAVE_STRUCT_TIMESPEC 1
#endif
#include <pthread.h>

namespace wav2mp3
{
class mutex::impl
{
  pthread_mutex_t mutex_;

public:
  impl()
  {
    int const errorCode = pthread_mutex_init(&mutex_, nullptr);

    if (errorCode)
      throw new std::system_error(errorCode, std::system_category());
  }
  ~impl()
  {
    pthread_mutex_destroy(&mutex_);
  }

  void lock()
  {
    pthread_mutex_lock(&mutex_);
  }
  void unlock()
  {
    pthread_mutex_unlock(&mutex_);
  }
};

mutex::mutex()
  : impl_{ std::make_unique<impl>() }
{
}

mutex::~mutex() = default;

void mutex::lock()
{
  impl_->lock();
}

void mutex::unlock()
{
  impl_->unlock();
}
}
