#include <memory>

namespace wav2mp3 {
class mutex
{
public:
  mutex();
  mutex(mutex const& other) = delete;
  mutex& operator=(mutex const& other) = delete;
  ~mutex();

  void lock();
  void unlock();

private:
  class impl;
  std::unique_ptr<impl> impl_;
};
}
