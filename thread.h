#include <functional>
#include <memory>

namespace wav2mp3 {
class thread
{
public:
  explicit thread(std::function<void()> const& f);
  thread(thread&& other);
  thread(thread const& other) = delete;
  thread& operator=(thread&& other);
  thread& operator=(thread const& other) = delete;
  ~thread();

  static unsigned hardware_concurrency();

private:
  class impl;
  std::unique_ptr<impl> impl_;
};
}
