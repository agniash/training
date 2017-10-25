#include "merge.h"
#include "../range/range.h"

#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iterator>

void test()
{
    std::vector<int> v1, v2;
    for (decltype(v1)& v : {std::ref(v1), std::ref(v2)}) {
        auto size = 100 + std::rand() % 100;
        v.resize(size);
        std::generate(v.begin(), v.end(), [size]{ return std::rand() % (1 + size / 2); });
        std::sort(v.begin(), v.end());
    }

    std::vector<int> dst1, dst2;
    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dst1));
    Merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dst2));
    ASSERT_EQ(dst1, dst2);
}

TEST(MergeTest, Test)
{
    for (auto i : Range(100)) {
        test();
    }
}
