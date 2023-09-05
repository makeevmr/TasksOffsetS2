#include "move_to_front.h"

void fillCodes(unsigned char *codes) {
    unsigned char code = 255;
    do {
        ++code;
        codes[code] = code;
    } while (code != 255);
}

unsigned char findAndMoveValue(unsigned char *codes, const unsigned char symbol) {
    unsigned char symbol_index = 0;
    unsigned char symbol_buffer = codes[0];
    while (codes[symbol_index] != symbol) {
        std::swap(symbol_buffer, codes[symbol_index]);
        ++symbol_index;
    }
    std::swap(symbol_buffer, codes[symbol_index]);
    std::swap(symbol_buffer, codes[0]);
    return symbol_index;
}

void moveValueFront(unsigned char *codes, int index) {
    int steps = index;
    for (int i = 0; i < steps; ++i) {
        std::swap(codes[index - 1], codes[index]);
        --index;
    }
}

void moveToFrontEncode(const char *input_file_name, const char *output_file_name) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    std::ofstream output_file(output_file_name, std::ios::binary);
    if (input_file.is_open() && output_file.is_open()) {
        const size_t codes_length = 256;
        unsigned char codes[codes_length];
        fillCodes(codes);
        unsigned char compression_methods = 1;
        output_file << compression_methods;
        unsigned char symbol_index;
        unsigned int i = 0;
        while (input_file.peek() != EOF) {
            unsigned char symbol = static_cast<unsigned char>(input_file.get());
            symbol_index = findAndMoveValue(codes, symbol);
            output_file << static_cast<unsigned char>(symbol_index);
            ++i;
        }
    }
    input_file.close();
    output_file.close();
}

void moveToFrontDecode(const char *input_file_name, const char *output_file_name) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    std::ofstream output_file(output_file_name, std::ios::binary);
    if (input_file.is_open() && output_file.is_open()) {
        const size_t codes_length = 256;
        unsigned char codes[codes_length];
        fillCodes(codes);
        input_file.get(); // delete first byte
        while (input_file.peek() != EOF) {
            unsigned char symbol = static_cast<unsigned char>(input_file.get());
            output_file << codes[symbol];
            moveValueFront(codes, static_cast<int>(symbol));
        }
    }
    input_file.close();
    output_file.close();
}