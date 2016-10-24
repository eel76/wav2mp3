#include <iosfwd>
#include <vector>

namespace wav2mp3 {

class pcm
{
public:
  struct sample
  {
    short left;
    short right;
  };

  pcm()
    : samples_{}
    , samples_per_second_{ 0 }
  {
  }
  ~pcm() = default;

  std::vector<sample> const& samples() const { return samples_; }

  int samples_per_second() const { return samples_per_second_; }

  friend std::istream& operator>>(std::istream& istr, pcm& data);

private:
  std::vector<sample> samples_;
  int samples_per_second_;
};
}
