#include "lock_guard.h"
#include "mutex.h"
#include "thread.h"
#include "gtest/gtest.h"

#include <memory>

using namespace std;
using namespace wav2mp3;

void
delay()
{
  for (int i = 0; i < 100; ++i)
    auto mem = make_unique<char[]>(256);
}

TEST(Mutex, IsLockable)
{
  mutex m;

  lock_guard<mutex> guard(m);
}

TEST(Mutex, IsOftenLockable)
{
  mutex m;

  auto guard = make_unique<lock_guard<mutex>>(m);
  for (int i = 0; i < 10; ++i) {
    guard.reset();
    guard = make_unique<lock_guard<mutex>>(m);
  }
}

TEST(Mutex, SynchronizesReadingThread)
{
  mutex m;

  auto value{ 0 };
  auto guard{ make_unique<lock_guard<mutex>>(m) };

  thread t{ [&]() {
    auto wait = make_unique<lock_guard<mutex>>(m);
    ASSERT_EQ(value, 1);
  } };

  delay();
  ++value;
  guard.reset();
}

TEST(Mutex, SynchronizesWritingThread)
{
  mutex m;

  auto value{ 0 };
  auto guard{ make_unique<lock_guard<mutex>>(m) };

  thread t{ [&]() {
    delay();

    ASSERT_EQ(value, 0);
    guard.reset();
  } };

  auto wait = make_unique<lock_guard<mutex>>(m);
  ++value;
}
