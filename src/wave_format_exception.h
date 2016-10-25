#include <exception>
#include <string>

namespace wav2mp3 {
class wave_format_exception : public std::exception
{
public:
  explicit wave_format_exception(char const* what)
    : what_{ what }
  {
  }
  char const* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};
}
