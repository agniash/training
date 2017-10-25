#pragma once

#include "../partition/partition.h"
#include "../insertion_sort/insertion_sort.h"

#include <iterator>
#include <functional>
#include <utility>
#include <cstdlib>
#include <stack>
#include <tuple>

namespace NQuickSortTraits {
    const size_t QuadraticSortCount = 7;
}

template <class Iterator, class Comparator = std::less<decltype(*Iterator())>>
void QuickSort(Iterator first, Iterator last, const Comparator& comparator = Comparator())
{
    while (true) {
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
        if (std::distance(first, middle) < std::distance(middle, last)) {
            QuickSort(first, middle);
            first = middle;
        } else {
            QuickSort(middle, last);
            last = middle;
        }
    }
}

template <class Iterator, class Comparator = std::less<decltype(*Iterator())>>
void IterativeQuickSort(Iterator first, Iterator last, const Comparator& comparator = Comparator())
{
    std::stack<std::pair<Iterator, Iterator>> stack;
    stack.push({first, last});

    while (!stack.empty()) {
        auto [left, right] = stack.top();
        stack.pop();

        auto count = std::distance(left, right);
        if (count < NQuickSortTraits::QuadraticSortCount) {
            InsertionSort(left, right, comparator);
            continue;
        }

        auto pivot = *std::next(left, std::rand() % count);
        auto predicate = [&](auto x) {
            return comparator(x, pivot);
        };
        Iterator middle = Partition(left, right, predicate);
        if (std::distance(left, middle) < std::distance(middle, right)) {
            stack.push({middle, right});
            stack.push({left, middle});
        } else {
            stack.push({left, middle});
            stack.push({middle, right});
        }
    }
}
