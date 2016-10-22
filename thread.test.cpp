#include "thread.h"
#include "gtest/gtest.h"

using namespace wav2mp3;

TEST(Thread, InvokesFunction)
{
  auto value = 0;
  {
    thread t{ [&]() { ++value; } };
  }
  ASSERT_EQ(value, 1);
}

TEST(Thread, IsMovable)
{
  auto value = 0;
  {
    thread t{ [&]() { ++value; } };
    thread u{ std::move(t) };
  }
  ASSERT_EQ(value, 1);
}
