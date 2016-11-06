#include "mp3_encoder.h"

namespace wav2mp3 {

class lame_encoder
  : public mp3_encoder
{
public:
  enum class encoding_quality : int
  {
    Good = 5
  };

  explicit lame_encoder(encoding_quality quality);
  virtual ~lame_encoder() = default;

  virtual std::vector<unsigned char> process(pcm const& source) const override;

private:
  encoding_quality quality_;
};
}
