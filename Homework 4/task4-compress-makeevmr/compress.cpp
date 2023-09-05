#include "algorithms/huffman.h"
#include "algorithms/lz_77.h"
#include "algorithms/move_to_front.h"
#include <string.h>

void decompress(const char *input_file_name, const char *output_file_name) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    unsigned char decompress_inf = static_cast<unsigned char>(input_file.get());
    input_file.close();
    bool mtf_d = false;
    bool lz77_d = false;
    bool huffman_d = false;
    if ((decompress_inf & (1 << 2)) != 0) {
        huffman_d = true;
    }
    if ((decompress_inf & (1 << 1)) != 0) {
        lz77_d = true;
    }
    if ((decompress_inf & 1) != 0) {
        mtf_d = true;
    }
    bool first_method = true;
    if (huffman_d) {
        if (mtf_d || lz77_d) {
            huffmanDecode(input_file_name, "input_file1.txt");
        } else {
            huffmanDecode(input_file_name, output_file_name);
        }
        first_method = false;
    }
    if (lz77_d) {
        if (first_method) {
            if (!mtf_d) {
                lz77Decode(input_file_name, output_file_name);
            } else {
                lz77Decode(input_file_name, "input_file1.txt");
            }
        } else {
            if (!mtf_d) {
                lz77Decode("input_file1.txt", output_file_name);
            } else {
                lz77Decode("input_file1.txt", "input_file2.txt");
            }
        }
        first_method = false;
    }
    if (mtf_d) {
        if (first_method) {
            moveToFrontDecode(input_file_name, output_file_name);
        } else if (huffman_d && lz77_d) {
            moveToFrontDecode("input_file2.txt", output_file_name);
        } else {
            moveToFrontDecode("input_file1.txt", output_file_name);
        }
    }
}

int main(int argc, char *argv[]) {
    bool mtf = false;
    bool lz77 = false;
    bool huffman = false;
    bool decompression = false;
    bool help = false;
    for (int i = 1; i < argc; ++i) {
        if (!mtf && (strcmp("-mtf", argv[i]) == 0)) {
            mtf = true;
        } else if (!lz77 && (strcmp("-lz77", argv[i]) == 0)) {
            lz77 = true;
        } else if (!huffman && (strcmp("-huffman", argv[i]) == 0)) {
            huffman = true;
        } else if (!decompression && (strcmp("-d", argv[i]) == 0)) {
            decompression = true;
        } else if (!help && ((strcmp("-h", argv[i]) == 0) || (strcmp("?", argv[i]) == 0))) {
            help = true;
        }
    }
    // compression part
    if (argc > 2) {
        const char *input_file_name = argv[argc - 2];
        const char *output_file_name = argv[argc - 1];
        bool first_method = true;
        if (mtf && (lz77 || huffman)) {
            moveToFrontEncode(input_file_name, "input_file1.txt");
            first_method = false;
        } else {
            moveToFrontEncode(input_file_name, output_file_name);
        }
        if (!first_method && lz77) {
            if (huffman) {
                lz77Encode("input_file1.txt", "input_file2.txt", false);
            } else {
                lz77Encode("input_file1.txt", output_file_name, false);
            }
        } else if (lz77) {
            if (huffman) {
                lz77Encode(input_file_name, "input_file1.txt");
            } else {
                lz77Encode(input_file_name, output_file_name);
            }
            first_method = false;
        }
        if (huffman) {
            if (first_method) {
                huffmanEncode(input_file_name, output_file_name);
            } else {
                if (mtf && lz77) {
                    huffmanEncode("input_file2.txt", output_file_name, false);
                } else {
                    huffmanEncode("input_file1.txt", output_file_name, false);
                }
            }
        }
        if (decompression) {
            decompress(input_file_name, output_file_name);
        }
    }
    if (help) {
        std::ifstream input_file("help.txt");
        if (input_file.is_open()) {
            while (input_file.peek() != EOF) {
                std::cout << static_cast<unsigned char>(input_file.get());
            }
        }
        input_file.close();
    }
}
