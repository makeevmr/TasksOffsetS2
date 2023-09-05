#include "MinHeap.h"

MinHeap::MinHeap(MinHeapNode *array, size_t length, size_t capacity)
    : _length(length), _capacity(capacity), _array(array) {
    int i = static_cast<int>((this->_length - 1) / 2);
    for (; i >= 0; --i) {
        this->minHeapify(i);
    }
}

MinHeap::MinHeap(const MinHeap &r_heap) {
    this->_length = r_heap._length;
    this->_capacity = r_heap._capacity;
    this->_array = new MinHeapNode[this->_capacity];
    for (size_t i = 0; i < this->_length; ++i) {
        this->_array[i] = r_heap._array[i];
    }
}

MinHeap &MinHeap::operator=(const MinHeap &r_heap) {
    if (this == &r_heap) {
        return *this;
    }
    delete[] this->_array;
    this->_length = r_heap._length;
    this->_capacity = r_heap._capacity;
    this->_array = new MinHeapNode[this->_capacity];
    for (size_t i = 0; i < this->_length; ++i) {
        this->_array[i] = r_heap._array[i];
    }
    return *this;
}

MinHeap::~MinHeap() {
    delete[] _array;
    _array = nullptr;
}

MinHeapNode MinHeap::extractMin() {
    MinHeapNode tmp = this->_array[0];
    this->_array[0] = this->_array[this->_length - 1];
    --this->_length;
    minHeapify(0);
    return tmp;
}

void MinHeap::minHeapify(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if ((left < this->_length) && (this->_array[smallest]._frequency > this->_array[left]._frequency)) {
        smallest = left;
    }
    if ((right < this->_length) && (this->_array[smallest]._frequency > this->_array[right]._frequency)) {
        smallest = right;
    }
    if (smallest != index) {
        std::swap(this->_array[smallest], this->_array[index]);
        minHeapify(smallest);
    }
}

void MinHeap::insertNode(const MinHeapNode inserted_node) {
    if (this->_length == this->_capacity) {
        this->_array = arrayResize<MinHeapNode>(this->_array, this->_capacity, this->_length);
    }
    ++this->_length;
    long long int index = static_cast<long long int>(this->_length - 1);
    while ((index > 0) && (inserted_node._frequency < this->_array[(index - 1) / 2]._frequency)) {
        this->_array[index] = this->_array[(index - 1) / 2];
        index = (index - 1) / 2;
    }
    this->_array[index] = inserted_node;
}

size_t MinHeap::getLength() const { return this->_length; }