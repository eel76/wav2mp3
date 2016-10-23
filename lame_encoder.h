#include "pcm.h"
#include <vector>

struct lame_global_struct;
typedef struct lame_global_struct lame_global_flags;

namespace wav2mp3 {

class lame_encoder
{
public:
  lame_encoder(int samples_per_second, int channels, int quality);
  ~lame_encoder() = default;

  std::vector<unsigned char> process(std::vector<pcm::sample> samples);

private:
  class flags
  {
  public:
    flags();
    flags(flags const& other) = delete;
    flags& operator=(flags const& other) = delete;
    ~flags();

    operator lame_global_flags*() const
    {
      return lame_global_flags_;
    }
  private:
    lame_global_flags* lame_global_flags_;
  };

  flags flags_;
};
}
