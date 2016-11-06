#include <vector>

namespace wav2mp3 {

class pcm;

class mp3_encoder
{
public:
  mp3_encoder() = default;
  virtual ~mp3_encoder() = default;

  virtual std::vector<unsigned char> process(pcm const& source) const = 0;
};
}
