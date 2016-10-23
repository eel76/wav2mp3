#include "lame.h"
#include "lame_encoder_exception.h"

#include <lame.h>

namespace wav2mp3 {

lame_encoder::lame_encoder(int samples_per_second, int channels, int quality)
{
  if (lame_set_in_samplerate(flags_, samples_per_second) != 0)
    throw lame_encoder_exception{ "Unable to init encoder samplerate" };

  if (lame_set_num_channels(flags_, channels) != 0)
    throw lame_encoder_exception{ "Unable to init encoder channels" };

  if (lame_set_quality(flags_, quality) != 0)
    throw lame_encoder_exception{ "Unable to init encoder quality" };

  if (lame_init_params(flags_) != 0)
    throw lame_encoder_exception{ "Unable to init encoder quality" };
}

std::vector<unsigned char>
lame_encoder::process(std::vector<pcm::sample> samples)
{
  std::vector<unsigned char> buffer;
  buffer.resize(samples.size() * 5 / 4 + 7200);

  int const frame_bytes = lame_encode_buffer_interleaved(
    flags_, &(samples.data()->left), static_cast<int>(samples.size()),
    buffer.data(), static_cast<int>(buffer.size()));

  if (frame_bytes < 0)
    return{};

  std::vector<unsigned char> frames{ buffer.data(), buffer.data() + frame_bytes };

  int mp3size = lame_encode_flush(flags_, buffer.data(),
    static_cast<int>(buffer.size()));

  if (mp3size > 0)
    frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

  return frames;
}

lame_encoder::flags::flags()
  : lame_global_flags_{ lame_init() }
{
  if (!lame_global_flags_)
    throw lame_encoder_exception{ "Unable to init lame encoder" };
}
lame_encoder::flags::~flags()
{
  lame_close(lame_global_flags_);
}
}
