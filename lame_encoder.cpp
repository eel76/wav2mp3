#include "lame_encoder.h"
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

lame_encoder::flags::flags()
  : global_flags_{ lame_init() }
{
  if (!global_flags_)
    throw lame_encoder_exception{ "Unable to init lame encoder" };
}
lame_encoder::flags::~flags()
{
  lame_close(global_flags_);
}
}
