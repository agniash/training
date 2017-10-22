#include "vector.h"

#include <iostream>
#include <gtest/gtest.h>

TEST(VectorTest, IsEmpty)
{
    TVector<int> v;
    ASSERT_TRUE(v.IsEmpty());
    v.PushBack(0);
    ASSERT_FALSE(v.IsEmpty());
}

TEST(VectorTest, GetSize)
{
    const size_t size = 1000;
    TVector<int> v;
    for (size_t i = 0; i < size; ++i) {
        v.PushBack(i);
    }
    ASSERT_EQ(v.GetSize(), size);
}

TEST(VectorTest, GetCapacity)
{
    const size_t size = 1000;
    TVector<int> v;
    for (size_t i = 0; i < size; ++i) {
        ASSERT_LE(i, v.GetCapacity());
        ASSERT_LE(v.GetCapacity(), TVector<int>::GrowthFactor * i + 1);
        v.PushBack(i);
    }
}
