#include "huffman.h"
#include <cstddef>

// find frequency for every symbol and push it into array
MinHeapNode *makeMinHeapNodeArray(const char *input_file_name, size_t &min_heap_node_array_length,
                                  unsigned long long *frequency_array, size_t &min_heap_node_array_capacity,
                                  bool first_method) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    const size_t frequency_capacity = 256;
    for (size_t i = 0; i < frequency_capacity; ++i) {
        frequency_array[i] = 0;
    }
    MinHeapNode *min_heap_node_array;
    if (input_file.is_open()) {
        if (!first_method) {
            input_file.get();
        }
        while (input_file.peek() != EOF) {
            ++frequency_array[input_file.get()];
        }
        for (size_t i = 0; i < frequency_capacity; ++i) {
            if (frequency_array[i] != 0) {
                ++min_heap_node_array_length;
            }
        }
        min_heap_node_array_capacity = min_heap_node_array_length;
        min_heap_node_array = new MinHeapNode[min_heap_node_array_capacity];
        size_t array_index = 0;
        for (size_t i = 0; i < frequency_capacity; ++i) {
            if (frequency_array[i] != 0) {
                min_heap_node_array[array_index]._data = static_cast<unsigned char>(i);
                min_heap_node_array[array_index]._frequency = frequency_array[i];
                min_heap_node_array[array_index]._is_leaf_node = true;
                min_heap_node_array[array_index]._left = nullptr;
                min_heap_node_array[array_index]._right = nullptr;
                ++array_index;
            }
        }
    }
    input_file.close();
    return min_heap_node_array;
}

MinHeapNode buildHuffmanTree(const char *input_file_name, unsigned long long *frequency_array, bool first_method) {
    size_t min_heap_node_array_capacity = 10;
    size_t min_heap_node_array_length = 0;
    MinHeapNode *min_heap_node_array = makeMinHeapNodeArray(
        input_file_name, min_heap_node_array_length, frequency_array, min_heap_node_array_capacity, first_method);
    MinHeap min_heap = MinHeap(min_heap_node_array, min_heap_node_array_length, min_heap_node_array_capacity);
    while (min_heap.getLength() > 1) {
        MinHeapNode *left_node = new MinHeapNode;
        MinHeapNode *right_node = new MinHeapNode;
        *left_node = min_heap.extractMin();
        *right_node = min_heap.extractMin();
        MinHeapNode new_node = MinHeapNode();
        new_node._is_leaf_node = false;
        new_node._data = ' ';
        new_node._left = left_node;
        new_node._right = right_node;
        new_node._frequency = left_node->_frequency + right_node->_frequency;
        min_heap.insertNode(new_node);
    }
    return min_heap.extractMin();
}

void buildHuffmanCodes(const MinHeapNode &root, std::string *codes_array, std::string &current_code) {
    std::string current_code_copy_1 = current_code;
    std::string current_code_copy_2 = current_code;
    if (root._left != nullptr) {
        current_code_copy_1.push_back('0');
        buildHuffmanCodes(*(root._left), codes_array, current_code_copy_1);
    }
    if (root._right != nullptr) {
        current_code_copy_2.push_back('1');
        buildHuffmanCodes(*(root._right), codes_array, current_code_copy_2);
    }
    if (root._left == nullptr && root._right == nullptr) {
        codes_array[root._data] = current_code;
    }
}

void writeByte(std::ofstream &output_file, unsigned char &bit_buffer, short int &current_bit) {
    output_file << bit_buffer;
    current_bit = 7;
    bit_buffer = 0;
}

void writeHuffmanCodes(const char *input_file_name, const char *output_file_name, std::string *codes_array,
                       bool first_method, size_t max_length, unsigned long long additional_bits) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    std::ofstream output_file(output_file_name, std::ios::binary);
    if (input_file.is_open() && output_file.is_open()) {
        // writing additional information
        unsigned char compression_methods = 0;
        if (!first_method) {
            compression_methods = static_cast<unsigned char>(input_file.get());
        }
        compression_methods |= (1 << 2);
        short int length_bits_allocate = 1; // how many bits allocated for length of huffman code for single symbol
        size_t current_length = 1;
        while (current_length < max_length) {
            ++length_bits_allocate;
            current_length = (current_length + 1) * 2 - 1;
        }
        compression_methods |= (static_cast<unsigned char>(length_bits_allocate) << 3);
        output_file << compression_methods;
        unsigned char codes_array_size = 0;
        for (size_t i = 0; i < 256; ++i) {
            if (!codes_array[i].empty()) {
                ++codes_array_size;
            }
        }
        output_file << static_cast<unsigned char>(codes_array_size);
        unsigned char bit_buffer = 0;
        short int current_bit = 7;
        // write list of codes into the file [Symbol, huffman length, huffmanCode]
        for (size_t index = 0; index < 256; ++index) {
            if (codes_array[index].empty()) {
                continue;
            }
            short int symbol_bit = 7;
            unsigned char symbol = static_cast<unsigned char>(index);
            std::string huffman_code = codes_array[index];
            if (current_bit == symbol_bit) { // writing current character into the file
                output_file << symbol;
            } else {
                do {
                    if ((symbol & (1 << symbol_bit)) != 0) {
                        bit_buffer |= (1 << current_bit);
                    }
                    --current_bit;
                    --symbol_bit;
                    if (current_bit == -1) {
                        writeByte(output_file, bit_buffer, current_bit);
                    }
                } while (symbol_bit != -1);
            }
            // writing length of code for current character into the file
            unsigned char code_length = static_cast<unsigned char>(huffman_code.length());
            short int length_bit = static_cast<short int>(length_bits_allocate - 1);
            do {
                if ((code_length & (1 << length_bit)) != 0) {
                    bit_buffer |= (1 << current_bit);
                }
                --current_bit;
                --length_bit;
                if (current_bit == -1) {
                    writeByte(output_file, bit_buffer, current_bit);
                }
            } while (length_bit != -1);
            for (unsigned char bit : huffman_code) { // writing code into the file
                if (bit == '1') {
                    bit_buffer |= (1 << current_bit);
                }
                --current_bit;
                if (current_bit == -1) {
                    writeByte(output_file, bit_buffer, current_bit);
                }
            }
        }
        short int additional_bit = 2;
        unsigned char uc_additional_bits = (10 - current_bit + static_cast<unsigned char>(additional_bits)) % 8;
        do {
            if ((uc_additional_bits & (1 << additional_bit)) != 0) {
                bit_buffer |= (1 << current_bit);
            }
            --current_bit;
            --additional_bit;
            if (current_bit == -1) {
                writeByte(output_file, bit_buffer, current_bit);
            }
        } while (additional_bit != -1);
        // file encoding
        while (input_file.peek() != EOF) {
            unsigned char new_symbol = static_cast<unsigned char>(input_file.get());
            for (unsigned char bit : codes_array[new_symbol]) {
                if (bit == '1') {
                    bit_buffer |= (1 << current_bit);
                }
                --current_bit;
                if (current_bit == -1) {
                    writeByte(output_file, bit_buffer, current_bit);
                }
            }
        }
        if (current_bit != 7) {
            output_file << bit_buffer;
        }
    }
    input_file.close();
    output_file.close();
}

void huffmanEncode(const char *input_file_name, const char *output_file_name, bool first_method) {
    const size_t capacity = 256;
    unsigned long long int frequency_array[capacity];
    std::string codes_array[capacity];
    MinHeapNode root = buildHuffmanTree(input_file_name, frequency_array, first_method);
    root._is_root = true;
    std::string current_code;
    if (root._is_leaf_node) {
        codes_array[root._data] = "0";
    } else {
        buildHuffmanCodes(root, codes_array, current_code);
    }
    size_t max_length = 0;
    unsigned long long additional_bits = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (frequency_array[i] > 0) {
            additional_bits = (frequency_array[i] * codes_array[i].length() + additional_bits) % 8;
            if (max_length < codes_array[i].length()) {
                max_length = codes_array[i].length();
            }
        }
    }
    writeHuffmanCodes(input_file_name, output_file_name, codes_array, first_method, max_length, additional_bits);
    if (!root._is_leaf_node) {
        deleteHuffmanTree(root);
    }
}

void deleteHuffmanTree(MinHeapNode &root) {
    if (root._left != nullptr) {
        deleteHuffmanTree(*(root._left));
    }
    if (root._right != nullptr) {
        deleteHuffmanTree(*(root._right));
    }
    if (!root._is_root) {
        delete &root;
    }
}

void buildHufffmanCodesTree(std::ifstream &input_file, int unique_symbols, unsigned char bits_for_length,
                            HuffmanCodesTree &huffman_tree, unsigned char &reading_byte, short int &reading_byte_bit) {
    unsigned long long int pow_2 = 1;
    for (unsigned char i = 1; i < bits_for_length; ++i) {
        pow_2 *= 2;
    }
    while (unique_symbols > 0) {
        unsigned long long int read_bits = 0;
        unsigned char character_byte = 0;
        short int character_byte_bit = 7;
        while (read_bits < 8) {
            if (reading_byte_bit == -1) {
                reading_byte = static_cast<unsigned char>(input_file.get());
                reading_byte_bit = 7;
            }
            if ((reading_byte & (1 << reading_byte_bit)) != 0) {
                character_byte |= (1 << character_byte_bit);
            }
            --reading_byte_bit;
            --character_byte_bit;
            ++read_bits;
        }
        read_bits = 0;
        unsigned long long int code_length = 0;
        unsigned long long int pow_2_copy = pow_2;
        while (read_bits < bits_for_length) {
            if (reading_byte_bit == -1) {
                reading_byte = static_cast<unsigned char>(input_file.get());
                reading_byte_bit = 7;
            }
            if ((reading_byte & (1 << reading_byte_bit)) != 0) {
                code_length += pow_2_copy;
            }
            --reading_byte_bit;
            pow_2_copy /= 2;
            ++read_bits;
        }
        read_bits = 0;
        while (read_bits < code_length) {
            ++read_bits;
            if (reading_byte_bit == -1) {
                reading_byte = static_cast<unsigned char>(input_file.get());
                reading_byte_bit = 7;
            }
            if ((reading_byte & (1 << reading_byte_bit)) != 0) {
                if (read_bits < code_length) {
                    huffman_tree.insert('1');
                } else {
                    huffman_tree.insert('1', character_byte, true);
                }
            } else {
                if (read_bits < code_length) {
                    huffman_tree.insert('0');
                } else {
                    huffman_tree.insert('0', character_byte, true);
                }
            }
            --reading_byte_bit;
        }
        --unique_symbols;
    }
}

void huffmanDecode(const char *input_file_name, const char *output_file_name) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    std::ofstream output_file(output_file_name, std::ios::binary);
    if (input_file.is_open() && output_file.is_open()) {
        unsigned char compression_methods = static_cast<unsigned char>(input_file.get());
        unsigned char bits_for_length = (compression_methods >> 3);
        if ((compression_methods & 3) != 0) {
            compression_methods &= 3;
            output_file << compression_methods;
        }
        int unique_symbols = input_file.get();
        if (unique_symbols == 0) {
            unique_symbols = 256;
        }
        HuffmanCodesTree huffman_tree;
        unsigned char reading_byte;
        short int reading_byte_bit = -1;
        buildHufffmanCodesTree(input_file, unique_symbols, bits_for_length, huffman_tree, reading_byte,
                               reading_byte_bit);
        unsigned long long int read_bits = 0;
        unsigned char additional_bits = 0;
        while (read_bits < 3) {
            if (reading_byte_bit == -1) {
                reading_byte = static_cast<unsigned char>(input_file.get());
                reading_byte_bit = 7;
            }
            ++read_bits;
            if ((reading_byte & (1 << reading_byte_bit)) != 0) {
                additional_bits |= (1 << (3 - read_bits));
            }
            --reading_byte_bit;
        }
        while (input_file.peek() != EOF) {
            if (reading_byte_bit == -1) {
                reading_byte = static_cast<unsigned char>(input_file.get());
                reading_byte_bit = 7;
            }
            if ((reading_byte & (1 << reading_byte_bit)) != 0) {
                huffman_tree.updateStauts('1');
            } else {
                huffman_tree.updateStauts('0');
            }
            if (huffman_tree.getCurrentNode()->_leaf_node) {
                output_file << huffman_tree.getCurrentNode()->_data;
                huffman_tree.resetCurrentPosition();
            }
            --reading_byte_bit;
        }
        while (((7 - reading_byte_bit) < additional_bits) || (additional_bits == 0 && reading_byte_bit >= 0)) {
            if ((reading_byte & (1 << reading_byte_bit)) != 0) {
                huffman_tree.updateStauts('1');
            } else {
                huffman_tree.updateStauts('0');
            }
            if (huffman_tree.getCurrentNode()->_leaf_node) {
                output_file << huffman_tree.getCurrentNode()->_data;
                huffman_tree.resetCurrentPosition();
            }
            --reading_byte_bit;
        }
    }
    input_file.close();
    output_file.close();
}
