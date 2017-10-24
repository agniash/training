#include "quick_sort.h"

#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>

void test(size_t size)
{
    std::vector<int> v1(size);
    std::iota(v1.begin(), v1.end(), 0);
    std::random_shuffle(v1.begin(), v1.end());
    auto v2 = v1;
    auto v3 = v1;

    std::sort(v1.begin(), v1.end());
    QuickSort(v2.begin(), v2.end());
    IterativeQuickSort(v3.begin(), v3.end());

    ASSERT_EQ(v1, v2);
    ASSERT_EQ(v1, v3);
}

TEST(QuickSortTest, TestZero)
{
    test(0);
}

TEST(QuickSortTest, TestOne)
{
    test(1);
}

TEST(QuickSortTest, TestTwo)
{
    test(2);
}

TEST(QuickSortTest, TestMany)
{
    for (size_t i = 0; i < 1000; ++i) {
        test(std::rand() % 1000);
    }
}
