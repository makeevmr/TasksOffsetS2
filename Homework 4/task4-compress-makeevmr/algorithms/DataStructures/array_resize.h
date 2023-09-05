#ifndef COMPRESS_ARRAY_RESIZE
#define COMPRESS_ARRAY_RESIZE

#include <fstream>

template <typename T>
T *arrayResize(T *array, size_t &capacity, size_t length) {
    capacity *= 2;
    T *tmp_array = new T[capacity];
    for (int i = 0; i < length; ++i) {
        tmp_array[i] = array[i];
    }
    delete[] array;
    return tmp_array;
}

#endif
