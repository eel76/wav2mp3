#include <exception>
#include <string>

namespace wav2mp3 {
class lame_encoder_exception : public std::exception
{
public:
  explicit lame_encoder_exception(char const* what)
    : what_{ what }
  {
  }
  char const* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};
}
