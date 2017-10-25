#pragma once

#include <functional>
#include <utility>
#include <algorithm>

template <
    class InputIterator1,
    class InputIterator2,
    class OutputIterator,
    class Comparator = std::less<decltype(*InputIterator1())>
>
OutputIterator Merge(
    InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2,
    OutputIterator dst, const Comparator& comparator = Comparator()
)
{
    while (first1 < last1 && first2 < last2) {
        *dst++ = comparator(*first1, *first2) ? *first1++ : *first2++;
    }
    dst = std::copy(first1, last1, dst);
    dst = std::copy(first2, last2, dst);
    return dst;
}

