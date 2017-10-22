#pragma once

#include <cstdlib>
#include <algorithm>

template <class T>
class TVector {
public:
    constexpr static float GrowthFactor = 1.5;
    constexpr static size_t StartCapacity = 1;

    TVector()
        : Capacity(0)
        , Size(0)
    {
        Reserve(StartCapacity);
    }

    ~TVector() {
        delete[] Buffer;
    }

    const T& operator [] (size_t index) const {
        return Buffer[index];
    }

    T& operator [] (size_t index) {
        return Buffer[index];
    }

    void PushBack(const T& value) {
        if (Size == Capacity) {
            size_t newCapacity = size_t(Capacity * GrowthFactor) + 1;
            Reserve(newCapacity);
        }

        Buffer[Size++] = value;
    }

    void Resize(size_t size, const T& value = T()) {
        if (size <= Size) {
            Size = size;
            return;
        }

        Reserve(size);
        std::fill(Buffer + Size, Buffer + size, value);
        Size = size;
    }

    void Reserve(size_t capacity) {
        if (capacity <= Capacity) {
            return;
        }

        if (T* newBuffer = (T*) std::realloc(Buffer, capacity * sizeof(T))) {
            Buffer = newBuffer;
            Capacity = capacity;
        } else {
            throw std::bad_alloc();
        }
    }

    bool IsEmpty() const {
        return Size == 0;
    }

    size_t GetSize() const {
        return Size;
    }

    size_t GetCapacity() const {
        return Capacity;
    }

    class TIterator {
    public:
        TIterator(T* ptr)
            : Ptr(ptr)
        {}

        T& operator * () {
            return *Ptr;
        }

        TIterator& operator ++ () {
            ++Ptr;
            return *this;
        }

        bool operator != (const TIterator& other) const {
            return Ptr != other.Ptr;
        }

    private:
        T* Ptr;
    };

    TIterator begin() {
        return TIterator(Buffer);
    }

    TIterator end() {
        return TIterator(Buffer + Size);
    }

private:
    T* Buffer = nullptr;
    size_t Size;
    size_t Capacity;
};
