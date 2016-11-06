#include "lame_encoder.h"
#include "mp3.h"

#include <ostream>

using namespace std;

namespace wav2mp3 {

mp3::mp3(pcm const& source)
  : frames_{ lame_encoder{ source.samples_per_second(),
                           source.number_of_channels(),
                           lame_encoder::encoding_quality::Good }
               .process(source.samples()) }
{
}

ostream&
operator<<(ostream& ostr, mp3 const& stream)
{
  return ostr.write(reinterpret_cast<char const*>(stream.frames_.data()),
                    stream.frames_.size());
}
}
