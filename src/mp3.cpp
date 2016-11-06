#include "mp3.h"
#include "mp3_encoder.h"

#include <ostream>

using namespace std;

namespace wav2mp3 {

mp3::mp3(pcm const& source, mp3_encoder const& encoder)
  : frames_{ encoder.process(source) }
{
}

ostream&
operator<<(ostream& ostr, mp3 const& stream)
{
  return ostr.write(reinterpret_cast<char const*>(stream.frames_.data()),
                    stream.frames_.size());
}
}
