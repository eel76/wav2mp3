#include "wav_files.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <experimental/filesystem>

using std::experimental::filesystem::directory_iterator;

namespace wav2mp3 {
namespace {
std::vector<path>
directory_entries(path const& directory)
{
  std::vector<path> entries;

  for (auto entry : directory_iterator(directory))
    entries.push_back(entry);

  return entries;
}
bool
has_wav_extension(path const& filename)
{
  path::string_type extension = filename.extension();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 ::tolower);

  return extension == path{ ".wav" };
}
bool
is_wav_file(path const& filename)
{
  std::array<char, 12> header;
  if (!std::ifstream{ filename, std::ifstream::binary }.read(header.data(),
                                                             header.size()))
    return false;

  if (std::string{ header.data() + 0, 4 } != "RIFF")
    return false;

  if (std::string{ header.data() + 8, 4 } != "WAVE")
    return false;

  return true;
}
}

std::vector<path>
wav_files(path const& directory)
{
  std::vector<path> files;

  for (auto entry : directory_entries(directory))
    if (has_wav_extension(entry) && is_wav_file(entry))
      files.push_back(entry);

  return files;
}
}
