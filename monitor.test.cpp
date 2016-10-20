#include "monitor.h"
#include "gtest/gtest.h"

using namespace wav2mp3;

TEST(Monitor, HandlesValues)
{
  monitor<int> counter{ 0 };
  counter([](int& value) { ++value; });
  int v = counter([](int& value) { return value; });
  ASSERT_EQ(v, 1);
}

TEST(Monitor, HandlesReferences)
{
  int v = 0;
  monitor<std::reference_wrapper<int>> counter{ v };
  counter([](int& value) { ++value; });
  ASSERT_EQ(v, 1);
}
