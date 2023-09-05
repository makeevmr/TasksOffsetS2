#ifndef COMPRESS_HUFFMAN_CODES_TREE
#define COMPRESS_HUFFMAN_CODES_TREE

#include <string>

class HuffmanCodesTree {
    struct HuffmanNode {
        bool _leaf_node;
        unsigned char _bit;
        unsigned char _data;
        HuffmanNode *_left;
        HuffmanNode *_right;

        HuffmanNode() : _leaf_node(false), _bit(0), _data(0), _left(nullptr), _right(nullptr) {}

        HuffmanNode(unsigned char bit, bool leaf_node, unsigned char data)
            : _leaf_node(leaf_node), _bit(bit), _data(data), _left(nullptr), _right(nullptr) {}

        ~HuffmanNode() {
            delete _left;
            delete _right;
            _left = nullptr;
            _right = nullptr;
        }
    };
    HuffmanNode *_root;
    HuffmanNode *_current_position;

public:
    HuffmanCodesTree() : _root(new HuffmanNode), _current_position(_root) {}

    void insert(unsigned char bit, unsigned char data = 0, bool leaf_node = false) {
        if (bit == '0') {
            if (_current_position->_left != nullptr) {
                _current_position = _current_position->_left;
            } else {
                HuffmanNode *new_node = new HuffmanNode(bit, leaf_node, data);
                _current_position->_left = new_node;
                if (!leaf_node) {
                    _current_position = new_node;
                } else {
                    _current_position = _root;
                }
            }
        } else {
            if (_current_position->_right != nullptr) {
                _current_position = _current_position->_right;
            } else {
                HuffmanNode *new_node = new HuffmanNode(bit, leaf_node, data);
                _current_position->_right = new_node;
                if (!leaf_node) {
                    _current_position = new_node;
                } else {
                    _current_position = _root;
                }
            }
        }
    }

    void updateStauts(unsigned char bit) {
        if (bit == '0' && (_current_position->_left != nullptr)) {
            _current_position = _current_position->_left;
        } else if (bit == '1' && (_current_position->_right != nullptr)) {
            _current_position = _current_position->_right;
        }
    }

    HuffmanNode *getCurrentNode() const { return _current_position; }

    void resetCurrentPosition() { _current_position = _root; }

    ~HuffmanCodesTree() { delete _root; }
};

#endif