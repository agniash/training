#include "insertion_sort.h"

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

    std::sort(v1.begin(), v1.end());
    InsertionSort(v2.begin(), v2.end());

    ASSERT_EQ(v1, v2);
}

TEST(InsertionSortTest, TestZero)
{
    test(0);
}

TEST(InsertionSortTest, TestOne)
{
    test(1);
}

TEST(InsertionSortTest, TestTwo)
{
    test(2);
}

TEST(InsertionSortTest, TestMany)
{
    for (size_t i = 0; i < 100; ++i) {
        test(std::rand() % 100);
    }
}
