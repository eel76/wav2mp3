#include "lame.h"
#include "lame_encoder_exception.h"
#include "pcm.h"

#include <lame.h>

namespace wav2mp3 {

std::vector<unsigned char> lame::encode(pcm const& source, int quality)
{
  lame encoder;

  encoder.init(source.samples_per_second(), source.channels(), quality);

  std::vector<pcm::sample> samples = source.samples();

  std::vector<unsigned char> buffer;
  buffer.resize(source.samples().size() * 5 / 4 + 7200);

  int mp3size = lame_encode_buffer_interleaved(
    encoder.flags_, &(samples.data()->left), static_cast<int>(samples.size()),
    buffer.data(), static_cast<int>(buffer.size()));

  if (mp3size < 0)
    return{};

  std::vector<unsigned char> frames;
  frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

  mp3size =
    lame_encode_flush(encoder.flags_, buffer.data(), static_cast<int>(buffer.size()));

  if (mp3size > 0)
    frames.insert(frames.end(), buffer.data(), buffer.data() + mp3size);

  return frames;
}

lame::lame()
  : flags_{ lame_init() }
{
  if (!flags_)
    throw lame_encoder_exception{ "Unable to init lame encoder" };
}
lame::~lame()
{
  lame_close(flags_);
}

void lame::init(int samples_per_second, int channels, int quality)
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
}
