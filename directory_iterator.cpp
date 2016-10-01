#include "directory_iterator.h"

#include <dirent.h>

namespace wav2mp3 {
directory_iterator::directory_iterator()
  : directory_{}
  , entry_{ nullptr }
{
}

directory_iterator::directory_iterator(char const* path)
  : directory_{ opendir(path), ::closedir }
  , entry_{ nullptr }
{
  next();
}

directory_iterator::~directory_iterator()
{
  entry_ = nullptr;
  directory_ = nullptr;
}

bool
directory_iterator::operator==(directory_iterator const& other) const
{
  return (directory_ == other.directory_) && (this->entry_ == other.entry_);
}

bool
directory_iterator::operator!=(directory_iterator const& other) const
{
  return !operator==(other);
}

char const* directory_iterator::operator*() const
{
  return entry_->d_name;
}

directory_iterator& directory_iterator::operator++()
{
  next();
  return *this;
}

void
directory_iterator::next()
{
  if (!directory_)
    return;

  entry_ = readdir(directory_.get());

  if (!entry_)
    directory_ = nullptr;
}

directory_iterator
begin(directory_iterator itr)
{
  return itr;
}

directory_iterator
end(directory_iterator const&)
{
  return directory_iterator();
}
}
