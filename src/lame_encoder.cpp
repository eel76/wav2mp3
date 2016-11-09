#include "lame_encoder.h"
#include "lame_encoder_exception.h"
#include "pcm.h"

#include <lame.h>

#include <memory>

using namespace std;

namespace wav2mp3 {

using encoder_engine = lame_global_flags;

namespace {
struct delete_engine
{
  void operator()(encoder_engine* garbage) const { lame_close(garbage); }
};

auto
make_engine(pcm::samplerate                samples_per_second,
            pcm::channels                  number_of_channels,
            lame_encoder::encoding_quality quality)
{
  unique_ptr<encoder_engine, delete_engine> engine(lame_init());

  if (!engine)
    throw lame_encoder_exception{ "Unable to init encoder engine" };

  if (lame_set_in_samplerate(engine.get(), samples_per_second) != 0)
    throw lame_encoder_exception{ "Unable to init samplerate" };

  if (lame_set_num_channels(engine.get(),
                            static_cast<int>(number_of_channels)) != 0)
    throw lame_encoder_exception{ "Unable to init number of channels" };

  if (lame_set_quality(engine.get(), static_cast<int>(quality)) != 0)
    throw lame_encoder_exception{ "Unable to init quality" };

  if (lame_init_params(engine.get()) != 0)
    throw lame_encoder_exception{ "Unable to init parameters" };

  return engine;
}

vector<unsigned char>
encode(encoder_engine* engine, pcm::channels number_of_channels,
       std::vector<pcm::sample> samples)
{
  vector<unsigned char> buffer;
  buffer.resize(samples.size() * 5 / 4 + 7200);

  int encoded_size = 0;

  if (number_of_channels == pcm::channels::Mono)
    encoded_size = lame_encode_buffer(
      engine, samples.data(), samples.data(), static_cast<int>(samples.size()),
      buffer.data(), static_cast<int>(buffer.size()));

  if (number_of_channels == pcm::channels::Stereo)
    encoded_size = lame_encode_buffer_interleaved(
      engine, samples.data(), static_cast<int>(samples.size() / 2),
      buffer.data(), static_cast<int>(buffer.size()));

  if (encoded_size < 0)
    return {};

  encoded_size +=
    lame_encode_flush(engine, buffer.data() + encoded_size,
                      static_cast<int>(buffer.size() - encoded_size));

  return { buffer.begin(), buffer.begin() + encoded_size };
}
}

lame_encoder::lame_encoder(encoding_quality quality)
  : quality_{ quality }
{
}

vector<unsigned char>
lame_encoder::process(pcm const& source) const
{
  auto const samples_per_second = source.samples_per_second();
  auto const number_of_channels = source.number_of_channels();

  auto const engine =
    make_engine(samples_per_second, number_of_channels, quality_);
  return encode(engine.get(), number_of_channels, source.samples());
}
}
