#pragma once

#include "../merge/merge.h"

#include <functional>
#include <algorithm>
#include <type_traits>

template <class Iterator>
using Deref = typename std::decay<decltype(*Iterator())>::type;

template <class Iterator, class Comparator = std::less<Deref<Iterator>>>
void MergeSort(Iterator first, Iterator last, const Comparator& comparator = Comparator())
{
    auto size = std::distance(first, last);
    std::vector<Deref<Iterator>> buffer(size);
    for (size_t blockSize = 1; blockSize < size; blockSize *= 2) {
        auto begin = first;
        auto dst = buffer.begin();
        while (std::distance(begin, last) > blockSize) {
            auto middle = std::next(begin, blockSize);
            auto end = std::distance(middle, last) > blockSize ? std::next(middle, blockSize) : last;
            dst = Merge(begin, middle, middle, end, dst);
            begin = end;
        }
        std::copy(begin, last, dst);
        std::copy(buffer.begin(), buffer.end(), first);
    }
}
