#include <memory>

struct DIR;
struct dirent;

namespace wav2mp3 {
class directory_iterator
{
public:
  directory_iterator();
  explicit directory_iterator(char const* path);
  ~directory_iterator();

  bool operator==(directory_iterator const& other) const;
  bool operator!=(directory_iterator const& other) const;

  char const* operator*() const;

  directory_iterator& operator++();

private:
  void next();

  std::shared_ptr<DIR> directory_;
  struct dirent* entry_;
};

directory_iterator begin(directory_iterator itr);
directory_iterator end(directory_iterator const&);
}