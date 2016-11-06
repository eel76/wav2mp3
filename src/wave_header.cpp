#include "wave_format_exception.h"
#include "wave_header.h"

#include <istream>

using namespace std;

namespace wav2mp3 {

istream&
operator>>(istream& istr, wave_header& ret)
{
  wave_header header;

  if (!istr.read(header.riff, sizeof(header)))
    throw wave_format_exception("Unexpected end of file");

  if (string{ header.riff, sizeof(header.riff) } != "RIFF")
    throw wave_format_exception("RIFF header not found");

  if (string{ header.wave, sizeof(header.wave) } != "WAVE")
    throw wave_format_exception("WAVE header not found");

  if (string{ header.fmt, sizeof(header.fmt) } != "fmt ")
    throw wave_format_exception("FMT header not found");

  if (header.fmt_size != 16)
    throw wave_format_exception("Unexpected FMT header size");

  if (string{ header.data, sizeof(header.data) } != "data")
    throw wave_format_exception("DATA header not found");

  swap(header, ret);
  return istr;
}
}
