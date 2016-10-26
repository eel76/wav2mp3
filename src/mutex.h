#include <memory>

namespace wav2mp3 {
class mutex
{
public:
  mutex();
  mutex(mutex&& other);
  mutex& operator=(mutex&& other);
  ~mutex();

  void lock();
  void unlock();

private:
  class impl;
  std::unique_ptr<impl> impl_;
};
}
