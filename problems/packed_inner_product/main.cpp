#include "packed_inner_product.h"

#include <gtest/gtest.h>

Packed PackVector(const std::vector<decltype(Pack::Value)>& v)
{
    Packed packed;
    auto cur = v.front();
    decltype(Pack::Count) count = 0;
    for (const auto& x : v) {
        if (x != cur) {
            packed.push_back({cur, count});
            count = 1;
            cur = x;
        } else {
            ++count;
        }
    }
    packed.push_back({cur, count});
    return packed;
}

template <class T>
auto SimpleProduct(const std::vector<T>& first, const std::vector<T>& second)
{
    T product = 0;
    for (size_t i = 0; i < first.size(); ++i) {
        product += first[i] * second[i];
    }
    return product;
}

void test(size_t size)
{
    std::vector<decltype(Pack::Value)> firstVector, secondVector;
    for (size_t i = 0; i < size; ++i) {
        firstVector.push_back(std::rand() % (1 + size / 2));
        secondVector.push_back(std::rand() % (1 + size / 2));
    }
    auto firstPacked = PackVector(firstVector);
    auto secondPacked = PackVector(secondVector);
    auto packedProduct = PackedInnerProduct(firstPacked, secondPacked);
    auto simpleProduct = SimpleProduct(firstVector, secondVector);
    ASSERT_EQ(packedProduct, simpleProduct);
}

TEST(PackedInnerProductTest, TestOne)
{
    test(1);
}

TEST(PackedInnerProductTest, TestTwo)
{
    test(2);
}

TEST(PackedInnerProductTest, TestThree)
{
    test(3);
}

TEST(PackedInnerProductTest, TestMany)
{
    for (size_t i = 0; i < 1000; ++i) {
        test(1 + std::rand() % 1000);
    }
}
