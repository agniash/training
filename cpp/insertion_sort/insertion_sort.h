#pragma once

#include <functional>
#include <utility>
#include <iterator>

template <class Iterator, class Comparator = std::less<decltype(*std::declval<Iterator>())>>
void InsertionSort(Iterator first, Iterator last, const Comparator& comparator = Comparator())
{
    for (Iterator current = std::next(first); current < last; ++current) {
        auto currentValue = *current;
        Iterator it = std::prev(current);
        while (it >= first && !comparator(*it, currentValue)) {
            *std::next(it) = *it;
            --it;
        }
        *std::next(it) = currentValue;
    }
}
