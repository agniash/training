#pragma once

#include "../partition/partition.h"
#include "../insertion_sort/insertion_sort.h"

#include <iterator>
#include <functional>
#include <utility>
#include <cstdlib>

namespace NQuickSortTraits {
    const size_t QuadraticSortCount = 7;
}

template <class Iterator, class Comparator = std::less<decltype(*std::declval<Iterator>())>>
void QuickSort(Iterator first, Iterator last, const Comparator& comparator = Comparator())
{
    auto count = std::distance(first, last);
    if (count < NQuickSortTraits::QuadraticSortCount) {
        InsertionSort(first, last, comparator);
        return;
    }

    auto pivot = *std::next(first, std::rand() % count);
    auto predicate = [&](auto x) {
        return comparator(x, pivot);
    };
    Iterator middle = Partition(first, last, predicate);
    QuickSort(first, middle);
    QuickSort(middle, last);
}
