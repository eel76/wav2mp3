#include "mp3.h"
#include "lame_encoder.h"

#include <ostream>

namespace wav2mp3 {

mp3::mp3(pcm const& source)
  : frames_{ lame_encoder{ source.samples_per_second() }.process(source.samples()) }
{
}

std::ostream&
operator<<(std::ostream& ostr, mp3 const& data)
{
  return ostr.write(reinterpret_cast<char const*>(data.frames_.data()),
                    data.frames_.size());
}
}
