//
// Created by Matthew Good on 9/3/21.
//

#ifndef CIRCULARBUFFER_MINIMALARRAY_H
#define CIRCULARBUFFER_MINIMALARRAY_H

#include <algorithm>
using std::size_t;

template<typename T> class MinimalArray {


    T * array;
    size_t length;

public:

    size_t size() const noexcept {
        return length;
    }


    bool is_allocated() const noexcept {
        return array != nullptr;
    }

    explicit MinimalArray(size_t size) : length(size), array(new T[size]) {}


    MinimalArray(const MinimalArray &what) : MinimalArray(what.length) {
        for (int i = 0; i < length; ++i) this->operator[](i) = what[i];
    };

    MinimalArray(MinimalArray &&what) noexcept : length(what.length) {
        array = what.array;
        what.array = nullptr;
        what.length = 0;
    };

    ~MinimalArray() {
        delete[] array;
    }

    void operator=(const MinimalArray & what) {
        delete [] array;
        array = nullptr;
        array = new T[length];
        length = what.length;
        for (int i = 0; i < length; ++i) this->operator[](i) = what[i];
    }

    void operator=(MinimalArray && what) {
        delete [] array;
        array = nullptr;
        length = what.length;
        array = what.array;
        what.array = nullptr;
        what.length = 0;
    }

    void fill(const T & value) { fill(value, length); }

    void fill(const T & value, size_t length) { if (array == nullptr) return;
     std::fill(array, array + length, value);
    }

    T & operator[](size_t index) {
        if (array == nullptr) {
            throw std::length_error("array is of zero length (deallocated");
        }
        return array[index];
    }

    const T & operator[](size_t index) const {
        if (array == nullptr) {
            throw std::length_error("array is of zero length (deallocated");
        }
        return array[index];
    }
};

#endif //CIRCULARBUFFER_MINIMALARRAY_H
