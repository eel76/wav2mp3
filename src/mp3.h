#include <iosfwd>
#include <vector>

namespace wav2mp3 {

class mp3_encoder;
class pcm;

class mp3
{
public:
  mp3(pcm const& source, mp3_encoder const& encoder);
  ~mp3() = default;

  friend std::ostream& operator<<(std::ostream& ostr, mp3 const& stream);

private:
  std::vector<unsigned char> frames_;
};
}
