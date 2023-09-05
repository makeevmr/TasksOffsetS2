#ifndef COMPRESS_HUFFMAN
#define COMPRESS_HUFFMAN

#include "DataStructures/HuffmanCodesTree.h"
#include "DataStructures/MinHeap.h"

MinHeapNode *makeMinHeapNodeArray(const char *input_file_name, size_t &min_heap_node_array_length,
                                  unsigned long long *frequency_array, size_t &min_heap_node_array_capacity,
                                  bool first_method);

MinHeapNode buildHuffmanTree(const char *input_file_name, unsigned long long *frequency_array, bool first_method);

void buildHuffmanCodes(const MinHeapNode &root, std::string *codes_array, std::string &current_code);

void writeByte(std::ofstream &output_file, unsigned char &bit_buffer, short int &current_bit);

void writeHuffmanCodes(const char *input_file_name, const char *output_file_name, std::string *codes_array,
                       bool first_method, size_t max_length, unsigned long long additional_bits);

void huffmanEncode(const char *input_file_name, const char *output_file_name, bool first_method = true);

void deleteHuffmanTree(MinHeapNode &root);

void buildHufffmanCodesTree(std::ifstream &input_file, int unique_symbols, unsigned char bits_for_length,
                            HuffmanCodesTree &huffman_tree, unsigned char &reading_byte, short int &reading_byte_bit);

void huffmanDecode(const char *input_file_name, const char *output_file_name);

#endif
