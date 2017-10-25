/*
 * Packed vector - vector of pairs (Value, Count) - RLE-encoded vector of numbers.
 * Problem: Given two packed vectors get their inner product (i.e. inner product of the original vectors).
 * Memory: O(1)
 * Time: O(n)
 */

#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <string>

struct Pack {
    long long Value;
    size_t Count;
};

using Packed = std::vector<Pack>;

auto PackedInnerProduct(const Packed& firstPack, const Packed& secondPack)
{
    if (firstPack.empty() || secondPack.empty()) {
        throw std::string("empty packed vectors not allowed");
    }
    decltype(Pack::Value) product = 0;
    auto [first, second] = std::make_pair(firstPack.begin(), secondPack.begin());
    auto [firstCount, secondCount] = std::make_pair(first->Count, second->Count);
    while (first != firstPack.end() && second != secondPack.end()) {
        auto count = std::min(firstCount, secondCount);
        product += count * first->Value * second->Value;
        for (auto& p : {std::make_pair(std::ref(first), std::ref(firstCount)), std::make_pair(std::ref(second), std::ref(secondCount))}) {
            p.second -= count;
            if (p.second == 0) {
                ++p.first;
                p.second = p.first->Count;
            }
        }
    }
    if (first != firstPack.end() || second != secondPack.end()) {
        throw std::string("different length of initial vectors not allowed");
    }
    return product;
}
