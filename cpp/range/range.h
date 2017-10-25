#pragma once

#include <iterator>
#include <type_traits>

template <class T, class Enable = void>
class RangeTraits;

template <class T>
struct RangeTraits<T, typename std::enable_if<!std::is_arithmetic<T>::value>::type> {
    using StepType = typename std::iterator_traits<T>::difference_type;

    static void Advance(T& value, const StepType& step)
    {
        std::advance(value, step);
    }
};

template <class T>
struct RangeTraits<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    using StepType = T;

    static void Advance(T& value, const StepType& step)
    {
        value += step;
    }
};

template <class T>
class RangeClassBase {
public:
    using StepType = typename RangeTraits<T>::StepType;

    RangeClassBase(const T& first, const T& last, StepType step = 1)
        : First(first)
        , Last(last)
        , Step(step)
    {}

    class Iterator {
    public:
        Iterator(T value, const StepType& step)
            : Value(value)
            , Step(step)
        {}

        const T& operator * () const
        {
            return Value;
        }

        bool operator != (const Iterator& other) const
        {
            return Step > 0 ? Value < other.Value : Value > other.Value;
        }

        Iterator& operator ++ ()
        {
            RangeTraits<T>::Advance(Value, Step);
            return *this;
        }

    private:
        T Value;
        const StepType& Step;
    };

    Iterator begin() const
    {
        return Iterator(First, Step);
    }

    Iterator end() const
    {
        return Iterator(Last, Step);
    }

private:
    const T& First;
    const T& Last;
    StepType Step;
};

template <class T, class Enable = void> 
class RangeClass;

template <class T>
class RangeClass<T, typename std::enable_if<!std::is_arithmetic<T>::value>::type> : public RangeClassBase<T> {
    using RangeClassBase<T>::RangeClassBase;
};

template <class T>
class RangeClass<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> : public RangeClassBase<T> {
public:
    using RangeClassBase<T>::RangeClassBase;

    RangeClass(T last)
        : RangeClassBase<T>(0, last)
    {}
};

template <class T>
auto Range(const T& first, const T& last, typename RangeClass<T>::StepType step = 1)
{
    return RangeClass<T>(first, last, step);
}

template <class T>
auto Range(const T& last)
{
    return RangeClass<T>(last);
}
