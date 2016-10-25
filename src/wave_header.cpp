#include "wave_format_exception.h"
#include "wave_header.h"

#include <istream>

namespace wav2mp3 {

std::istream&
operator>>(std::istream& istr, wave_header& ret)
{
  wave_header header;

  if (!istr.read(header.riff, sizeof(header)))
    throw wave_format_exception("Unexpected end of file");

  if (std::string{ header.riff, sizeof(header.riff) } != "RIFF")
    throw wave_format_exception("RIFF header not found");

  if (std::string{ header.wave, sizeof(header.wave) } != "WAVE")
    throw wave_format_exception("WAVE header not found");

  if (std::string{ header.fmt, sizeof(header.fmt) } != "fmt ")
    throw wave_format_exception("FMT header not found");

  if (header.fmt_size != 16)
    throw wave_format_exception("Unexpected FMT header size");

  if (std::string{ header.data, sizeof(header.data) } != "data")
    throw wave_format_exception("DATA header not found");

  using namespace std;
  swap(header, ret);
  return istr;
}
}
