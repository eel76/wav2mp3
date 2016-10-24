#include <iosfwd>
#include <vector>

namespace wav2mp3 {

class pcm
{
public:
  typedef short sample;
  typedef int samplerate;

  enum class channels : int
  {
    Mono = 1,
    Stereo = 2
  };


  pcm()
    : samples_{}
    , samples_per_second_{ 44100 }
    , number_of_channels_{ channels::Stereo }
  {
  }
  ~pcm() = default;

  std::vector<sample> const& samples() const { return samples_; }

  samplerate samples_per_second() const { return samples_per_second_; }
  channels number_of_channels() const { return number_of_channels_; }

  friend std::istream& operator>>(std::istream& istr, pcm& data);

private:
  std::vector<sample> samples_;
  samplerate samples_per_second_;
  channels number_of_channels_;
};
}
