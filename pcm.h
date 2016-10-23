#include <vector>

namespace wav2mp3 {

class pcm
{
public:
  pcm()
    : samples_{}
    , samples_per_second_{ 0 }
  {
  }
  ~pcm() = default;

  struct sample
  {
    short left;
    short right;
  };
  std::vector<sample> const& samples() const
  {
    return samples_;
  }

  int channels() const
  {
    return 2;
  }
  int samples_per_second() const
  {
    return samples_per_second_;
  }

  friend std::istream& operator>> (std::istream& istr, pcm& data);

private:
  std::vector<sample> samples_;
  int samples_per_second_;
};
}
