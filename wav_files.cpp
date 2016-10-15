#include "wav_files.h"

#include "directory_iterator.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>

namespace wav2mp3 {
namespace {
bool
is_wav_file(std::string const& filename)
{
  // expected file name format: /.[.][wW][aA][vV]$/
  size_t const length = filename.length();
  if (length < /* sic! */ 5)
    return false;

  std::string extension = filename.substr(length - 4, length);
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

  if (extension != ".wav")
    return false;

  std::array<char, 12> header;
  if (! std::ifstream{ filename, std::ifstream::binary }.read(header.data(), header.size()))
    return false;

  if (std::string{ header.data() + 0, 4 } != "RIFF")
    return false;

  if (std::string{ header.data() + 8, 4 } != "WAVE")
    return false;

  return true;
}
}

std::vector<std::string>
directory_entries(char const* path)
{
  std::vector<std::string> entries;

  for (auto entry : directory_iterator(path))
    entries.push_back(std::string{ path } +'/' + entry);

  return entries;
}

std::vector<std::string>
wav_files(char const* path)
{
  std::vector<std::string> files;

  for (auto entry : directory_entries(path))
    if (is_wav_file(entry))
      files.push_back(entry);

  return files;
}
}
