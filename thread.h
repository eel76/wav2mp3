#include <functional>
#include <memory>

namespace wav2mp3
{
class thread
{
  class impl;
  std::unique_ptr<impl> impl_;

public:
  explicit thread(std::function<void()> const& f);
  thread(thread&& other);
  thread& operator=(thread&& other);
  ~thread();

  thread(thread const& other) = delete;
  thread& operator=(thread const& other) = delete;

  static unsigned hardware_concurrency();
};
}
