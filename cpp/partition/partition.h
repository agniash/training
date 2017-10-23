#pragma once

#include <iterator>
#include <algorithm>

template<class Iterator, class Predicate>
Iterator Partition(Iterator first, Iterator last, const Predicate& predicate)
{
    while (first < last && predicate(*first)) {
        ++first;
    }
    for (Iterator it = std::next(first); it < last; ++it) {
        if (predicate(*it)) {
            std::iter_swap(it, first++);
        }
    }
    return first;
}
