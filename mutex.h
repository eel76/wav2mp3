#include <memory>

namespace wav2mp3
{
class mutex
{
  class impl;
  std::unique_ptr<impl> impl_;

public:
  mutex();
  ~mutex();

  mutex(mutex const& other) = delete;
  mutex& operator=(mutex const& other) = delete;

  void lock();
  void unlock();
};
}
