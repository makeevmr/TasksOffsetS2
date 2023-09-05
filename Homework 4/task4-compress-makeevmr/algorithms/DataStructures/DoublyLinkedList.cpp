#include "DoublyLinkedList.h"

// Node definition
template <typename T>
Node<T>::Node() : _next(nullptr), _prev(nullptr) {}

template <typename T>
Node<T>::Node(T data) : _data(data), _next(nullptr), _prev(nullptr) {}

// DoublyLinkedList definition
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : _firstNode(nullptr), _lastNode(nullptr) {}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(T *data_array, size_t length) {
    _firstNode = nullptr;
    _lastNode = nullptr;
    for (size_t i = 0; i < length; ++i) {
        this->insertEnd(data_array[i]);
    }
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    Node<T> *current_node = _firstNode;
    while (current_node != nullptr) {
        Node<T> *next_node = current_node->_next;
        delete current_node;
        current_node = next_node;
    }
}

template <typename T>
void DoublyLinkedList<T>::insertBeggining(const T &data) {
    Node<T> *new_node = new Node<T>(data);
    new_node->_next = _firstNode;
    if (_firstNode) {
        _firstNode->_prev = new_node;
    }
    _firstNode = new_node;
    if (_lastNode == nullptr) {
        _lastNode = new_node;
    }
}

template <typename T>
void DoublyLinkedList<T>::insertEnd(const T &data) {
    Node<T> *new_node = new Node<T>(data);
    new_node->_prev = _lastNode;
    if (_lastNode) {
        _lastNode->_next = new_node;
    }
    _lastNode = new_node;
    if (_firstNode == nullptr) {
        _firstNode = new_node;
    }
}

template <typename T>
void DoublyLinkedList<T>::moveNodeFront(Node<T> &node) {
    if (node._prev != nullptr && node._next != nullptr) {
        node._prev->_next = node._next;
        node._next->_prev = node._prev;
    } else if (node._prev != nullptr) {
        node._prev->_next = nullptr;
        _lastNode = node._prev;
    }
    node._prev = nullptr;
    node._next = _firstNode;
    _firstNode->_prev = &node;
    _firstNode = &node;
}

template <typename T>
void DoublyLinkedList<T>::removeNode(const T &data) {
    Node<T> *current_node = _firstNode;
    while (current_node != nullptr) {
        if (current_node->_data == data) {
            if (current_node->_prev == nullptr) {
                _firstNode = current_node->_next;
            } else {
                current_node->_prev->_next = current_node->_next;
            }
            if (current_node->_next == nullptr) {
                _lastNode = current_node->_prev;
            } else {
                current_node->_next->_prev = current_node->_prev;
            }
            delete current_node;
            break;
        }
        current_node = current_node->_next;
    }
}

template <typename T>
void DoublyLinkedList<T>::printLinkedList() const {
    Node<T> *current_node = _firstNode;
    while (current_node != nullptr) {
        std::cout << current_node->_data << ' ';
        current_node = current_node->_next;
    }
    std::cout << '\n';
}

// Iterator definition
template <typename T>
DoublyLinkedList<T>::Iterator::Iterator(Node<T> *ptr) : _ptr(ptr) {}

template <typename T>
DoublyLinkedList<T>::Iterator::Iterator(const DoublyLinkedList<T>::Iterator &other) : _ptr(other._ptr) {}

template <typename T>
typename DoublyLinkedList<T>::Iterator &
DoublyLinkedList<T>::Iterator::operator=(const DoublyLinkedList<T>::Iterator &right) {
    if (this != &right) {
        this->_ptr = right._ptr;
    }
    return *this;
}

template <typename T>
T &DoublyLinkedList<T>::Iterator::operator*() const {
    return _ptr->_data;
}

template <typename T>
Node<T> *DoublyLinkedList<T>::Iterator::operator->() const {
    return _ptr;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator &DoublyLinkedList<T>::Iterator::operator++() {
    if (_ptr != nullptr) {
        _ptr = _ptr->_next;
    }
    return *this;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::begin() {
    return Iterator(_firstNode);
}

template <typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::end() {
    Iterator tmp(_lastNode);
    return ++tmp;
}

template class DoublyLinkedList<unsigned char>; // Template Instantiation of type int