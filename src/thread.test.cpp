#include "thread.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wav2mp3;

TEST(Thread, InvokesFunction)
{
  int value = 0;
  {
    thread t{ [&]() { ++value; } };
  }
  ASSERT_EQ(value, 1);
}

TEST(Thread, IsMovable)
{
  int value = 0;
  {
    thread t{ [&]() { ++value; } };
    thread u{ move(t) };
  }
  ASSERT_EQ(value, 1);
}
