#include <vector>

struct lame_global_struct;
typedef struct lame_global_struct lame_global_flags;

namespace wav2mp3 {

class pcm;

class lame
{
public:
  static std::vector<unsigned char> encode(pcm const& source, int quality);

private:
  lame();
  lame(lame const&) = delete;
  lame& operator=(lame const&) = delete;
  ~lame();

  void init(int samples_per_second, int channels, int quality);

  lame_global_flags* flags_;
};
}
