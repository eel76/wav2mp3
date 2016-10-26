#include "lock_guard.h"
#include "mutex.h"

namespace wav2mp3 {
auto
make_synchronize()
{
  return [m = std::move(mutex{})](auto& f) mutable
  {
    lock_guard<mutex> hold{ m };
    return f();
  };
}

template <class F>
auto
synchronize(F&& f)
{
  return [ f = std::forward<F>(f), sync = make_synchronize() ]() mutable
  {
    return sync(f);
  };
}
}
