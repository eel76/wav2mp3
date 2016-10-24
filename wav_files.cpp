#include "wav_files.h"
#include "wave_format_exception.h"
#include "wave_header.h"

#include <algorithm>
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
  wave_header header;

  try
  {
    if (std::ifstream{ filename, std::ifstream::binary } >> header)
      return true;
  }
  catch (wave_format_exception const&)
  {
  }

  return false;
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
