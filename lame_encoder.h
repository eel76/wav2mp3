#include "pcm.h"
#include <memory>
#include <vector>

struct lame_global_struct;
typedef struct lame_global_struct lame_global_flags;

namespace wav2mp3 {

class lame_encoder
{
public:
  enum class encoding_quality : int
  {
    Good = 5
  };

  lame_encoder(pcm::samplerate samples_per_second,
               pcm::channels number_of_channels, encoding_quality quality);
  ~lame_encoder() = default;

  std::vector<unsigned char> process(std::vector<pcm::sample> samples);

private:
  using encoder = lame_global_flags;
  struct delete_encoder
  {
    void operator()(encoder* garbage) const;
  };
  std::unique_ptr<encoder, delete_encoder> encoder_;
};
}
