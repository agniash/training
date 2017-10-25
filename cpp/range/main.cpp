#include "range.h"

#include <gtest/gtest.h>
#include <vector>

TEST(RangeTest, Test1)
{
    size_t i = 0;
    for (auto x : Range(10)) {
        ASSERT_EQ(x, i);
        ++i;
    }
}

TEST(RangeTest, Test2)
{
    size_t i = 10;
    for (auto x : Range(10, 20)) {
        ASSERT_EQ(x, i);
        ++i;
    }
}

TEST(RangeTest, Test3)
{
    size_t i = 10;
    for (auto x : Range(10, 20, 3)) {
        ASSERT_EQ(x, i);
        i += 3;
    }
}

TEST(RangeTest, Test4)
{
    size_t i = 20;
    for (auto x : Range(20, 10, -3)) {
        ASSERT_EQ(x, i);
        i -= 3;
    }
}

TEST(RangeTest, Test5)
{
    std::vector<int> v {1, 2, 3, 4, 5};
    size_t i = 0;
    for (const auto& it : Range(v.begin(), v.end())) {
        ASSERT_EQ(*it, v[i]);
        ++i;
    }
}
