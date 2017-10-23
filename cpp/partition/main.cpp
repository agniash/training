#include "partition.h"

#include <gtest/gtest.h>
#include <numeric>
#include <algorithm>

TEST(PartitionTest, Test)
{
    std::vector<int> v1(1000);
    std::iota(v1.begin(), v1.end(), 0);
    std::random_shuffle(v1.begin(), v1.end());
    auto v2 = v1;

    auto predicate = [](int x) {
        return x * x % 7 == 3;
    };

    auto it1 = std::partition(v1.begin(), v1.end(), predicate);
    auto it2 = Partition(v2.begin(), v2.end(), predicate);

    ASSERT_EQ(v1, v2);
    ASSERT_TRUE((it1 == v1.end() && it2 == v2.end()) || (*it1 == *it2));
}
