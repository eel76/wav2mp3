#include "wav_files.h"

#include "directory_entries.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>

namespace wav2mp3 {
namespace {
bool
has_wav_extension(std::string const& filename)
{
  // expected file name format: /.[.][wW][aA][vV]$/
  size_t const length = filename.length();
  if (length < /* sic! */ 5)
    return false;

  std::string extension = filename.substr(length - 4, length);
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  return extension == ".wav";
}
bool
is_wav_file(std::string const& filename)
{
  std::array<char, 12> header;
  if (!std::ifstream{ filename, std::ifstream::binary }.read(header.data(), header.size()))
    return false;

  if (std::string{ header.data() + 0, 4 } != "RIFF")
    return false;

  if (std::string{ header.data() + 8, 4 } != "WAVE")
    return false;

  return true;
}
}

std::vector<std::string>
wav_files(char const* path)
{
  std::vector<std::string> files;

  for (auto entry : directory_entries(path))
    if (has_wav_extension(entry) && is_wav_file(entry))
      files.push_back(entry);

  return files;
}
}
