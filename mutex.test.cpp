#include "lock_guard.h"
#include "mutex.h"
#include "thread.h"
#include "gtest/gtest.h"

#include <memory>

using namespace wav2mp3;

void
pause()
{
  for (int i = 0; i < 100; ++i)
    auto mem = std::make_unique<char[]>(256);
}

TEST(Mutex, IsLockable)
{
  mutex m;

  lock_guard<mutex> guard(m);
}

TEST(Mutex, IsOftenLockable)
{
  mutex m;

  auto guard = std::make_unique<lock_guard<mutex>>(m);
  for (int i = 0; i < 10; ++i) {
    guard.reset();
    guard = std::make_unique<lock_guard<mutex>>(m);
  }
}

TEST(Mutex, SynchronizesReadingThread)
{
  mutex m;

  auto value{ 0 };
  auto guard{ std::make_unique<lock_guard<mutex>>(m) };

  thread t{ [&]() {
    auto wait = std::make_unique<lock_guard<mutex>>(m);
    ASSERT_EQ(value, 1);
  } };

  pause();
  ++value;
  guard.reset();
}

TEST(Mutex, SynchronizesWritingThread)
{
  mutex m;

  auto value{ 0 };
  auto guard{ std::make_unique<lock_guard<mutex>>(m) };

  thread t{ [&]() {
    pause();

    ASSERT_EQ(value, 0);
    guard.reset();
  } };

  auto wait = std::make_unique<lock_guard<mutex>>(m);
  ++value;
}
