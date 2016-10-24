#include "pcm.h"
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

  explicit lame_encoder(int              samples_per_second,
                        encoding_quality quality = encoding_quality::Good);
  ~lame_encoder() = default;

  std::vector<unsigned char> process(std::vector<pcm::sample> samples);

private:
  class state
  {
  public:
    state();
    state(state const& other) = delete;
    state& operator=(state const& other) = delete;
    ~state();

    operator lame_global_flags*() const { return flags_; }
  private:
    lame_global_flags* flags_;
  };

  state state_;
};
}
