#include "lame_encoder.h"
#include "mp3.h"
#include "pcm.h"

#include <lame.h>

#include <ostream>

namespace wav2mp3 {

std::vector<unsigned char>
mp3::encode(pcm const& source)
{
  lame_encoder encoder{ source.samples_per_second(), source.channels(), 5 };

  std::vector<pcm::sample> samples = source.samples();

  std::vector<unsigned char> buffer;
  buffer.resize(source.samples().size() * 5 / 4 + 7200);

  int mp3size = lame_encode_buffer_interleaved(
    encoder, &(samples.data()->left), static_cast<int>(samples.size()),
    buffer.data(), static_cast<int>(buffer.size()));

  if (mp3size < 0)
    return {};

  std::vector<unsigned char> frames;
  frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

  mp3size =
    lame_encode_flush(encoder, buffer.data(), static_cast<int>(buffer.size()));

  if (mp3size > 0)
    frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

  return frames;
}

std::ostream&
operator<<(std::ostream& ostr, mp3 const& data)
{
  return ostr.write(reinterpret_cast<char const*>(data.frames_.data()),
                    data.frames_.size());
}
}
