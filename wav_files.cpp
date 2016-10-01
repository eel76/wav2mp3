#include "wav_files.h"

#include "directory_iterator.h"
#include <algorithm>
#include <cctype>

namespace wav2mp3 {
namespace {
bool
is_wav_file(std::string const& filename)
{
  // expected file name format: /.[.][wW][aA][vV]$/
  size_t const length = filename.length();
  if (length < 5)
    return false;

  std::string extension = filename.substr(length - 4, length);
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 ::tolower);

  return extension == ".wav";
}
}

std::vector<std::string>
wav_files(char const* path)
{
  std::vector<std::string> files;

  for (auto entry : directory_iterator(path))
    if (is_wav_file(entry) || true)
      files.push_back(std::string{ path } + '/' + entry);

  return files;
}
}
