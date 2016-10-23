#include <iosfwd>
#include <vector>

namespace wav2mp3 {

class pcm;

class mp3
{
public:
  explicit mp3(pcm const& source)
    : frames_{ encode(source) }
  {
  }
  ~mp3() = default;

  friend std::ostream& operator << (std::ostream& ostr, mp3 const& data);

private:
  static std::vector<unsigned char> encode(pcm const& source);

private:
  std::vector<unsigned char> frames_;
};
}
