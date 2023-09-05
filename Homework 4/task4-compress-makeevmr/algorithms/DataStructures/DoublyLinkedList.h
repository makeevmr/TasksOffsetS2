#ifndef COMPRESS_DOUBLY_LINKED_LIST
#define COMPRESS_DOUBLY_LINKED_LIST

#include <iostream>

template <typename T>
class Node {
public:
    T _data;
    Node<T> *_next;
    Node<T> *_prev;

    Node();

    Node(T data);
};

template <typename T>
class DoublyLinkedList {
    Node<T> *_firstNode;
    Node<T> *_lastNode;

public:
    DoublyLinkedList();

    DoublyLinkedList(T *data_array, size_t length);

    ~DoublyLinkedList();

    void insertBeggining(const T &data);

    void insertEnd(const T &data);

    void moveNodeFront(Node<T> &node);

    void removeNode(const T &data);

    void printLinkedList() const;

    class Iterator {
    private:
        Node<T> *_ptr;

    public:
        Iterator(Node<T> *ptr);

        Iterator(const Iterator &other);

        Iterator &operator=(const Iterator &right);

        T &operator*() const;

        Node<T> *operator->() const;

        Iterator &operator++();

        Iterator operator++(int);

        friend bool operator==(const typename DoublyLinkedList<T>::Iterator &left,
                               const typename DoublyLinkedList<T>::Iterator &right) {
            return left._ptr == right._ptr;
        }

        friend bool operator!=(const typename DoublyLinkedList<T>::Iterator &left,
                               const typename DoublyLinkedList<T>::Iterator &right) {
            return left._ptr != right._ptr;
        }
    };

    Iterator begin();

    Iterator end();
};

#endif
