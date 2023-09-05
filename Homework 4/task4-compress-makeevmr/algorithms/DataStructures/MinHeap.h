#include "array_resize.h"
#include <iostream>

struct MinHeapNode {
    bool _is_leaf_node = true;
    bool _is_root = false;
    unsigned char _data;
    unsigned long long _frequency;
    MinHeapNode *_left = nullptr;
    MinHeapNode *_right = nullptr;
};

class MinHeap {
    size_t _length;
    size_t _capacity;
    MinHeapNode *_array;

public:
    MinHeap(MinHeapNode *array, size_t length, size_t capacity);

    MinHeap(const MinHeap &r_heap);

    MinHeap &operator=(const MinHeap &r_heap);

    ~MinHeap();

    MinHeapNode extractMin();

    void minHeapify(int index);

    void insertNode(MinHeapNode inserted_node);

    size_t getLength() const;
};