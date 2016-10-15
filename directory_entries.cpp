#include "directory_entries.h"

#include "directory_iterator.h"

namespace wav2mp3 {

std::vector<std::string>
directory_entries(char const* path)
{
  std::vector<std::string> entries;

  for (auto entry : directory_iterator(path))
    entries.push_back(std::string{ path } + '/' + entry);

  return entries;
}
}
