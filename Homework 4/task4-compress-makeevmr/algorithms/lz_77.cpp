#include "lz_77.h"

void lz77Encode(const char *input_file_name, const char *output_file_name, bool first_method) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    std::ofstream output_file(output_file_name, std::ios::binary);
    if (input_file.is_open() && output_file.is_open()) {
        unsigned char offset = 0;
        unsigned char length = 0;
        unsigned char compression_methods = 0;
        if (!first_method) {
            compression_methods = static_cast<unsigned char>(input_file.get());
        }
        compression_methods |= (1 << 1);
        output_file << compression_methods;
        const int buffer_capacity = 128;
        unsigned char search_buffer[buffer_capacity];
        int left_index = 0;
        int right_index = -1;
        bool is_search_buffer_full = false;
        bool is_search_buffer_empty = true;
        size_t look_ahead_buffer_index = 0;
        size_t look_ahead_buffer_length = 0;
        unsigned char look_ahead_buffer[256];
        while (input_file.peek() != EOF) {
            length = 0;
            look_ahead_buffer_length = 0;
            int search_buffer_index = right_index + 1;
            // search for longest substring
            if (!is_search_buffer_empty) {
                do {
                    if (--search_buffer_index == -1) {
                        search_buffer_index = 127;
                    }
                    look_ahead_buffer_index = 0;
                    unsigned char current_length = 0;
                    int index_for_matching = search_buffer_index;
                    while ((look_ahead_buffer_index < look_ahead_buffer_length) &&
                           (look_ahead_buffer[look_ahead_buffer_index] == search_buffer[index_for_matching])) {
                        if (++current_length == 255) {
                            break;
                        }
                        ++index_for_matching;
                        ++look_ahead_buffer_index;
                        if (is_search_buffer_full && ((index_for_matching) % buffer_capacity == left_index) ||
                            !is_search_buffer_full && (index_for_matching > right_index)) {
                            index_for_matching = search_buffer_index;
                        } else if (index_for_matching == buffer_capacity) {
                            index_for_matching = 0;
                        }
                    }
                    if ((look_ahead_buffer_index == look_ahead_buffer_length) && (input_file.peek() != EOF)) {
                        unsigned char new_symbol = static_cast<unsigned char>(input_file.get());
                        look_ahead_buffer[look_ahead_buffer_length] = new_symbol;
                        ++look_ahead_buffer_length;
                        while (new_symbol == search_buffer[index_for_matching]) {
                            if (input_file.peek() != EOF) {
                                new_symbol = static_cast<unsigned char>(input_file.get());
                                look_ahead_buffer[look_ahead_buffer_length] = new_symbol;
                                ++look_ahead_buffer_length;
                            } else {
                                ++current_length;
                                break;
                            }
                            if (++current_length == 255) {
                                break;
                            }
                            ++index_for_matching;
                            if (is_search_buffer_full && ((index_for_matching) % buffer_capacity == left_index) ||
                                !is_search_buffer_full && (index_for_matching > right_index)) {
                                index_for_matching = search_buffer_index;
                            } else if (index_for_matching == buffer_capacity) {
                                index_for_matching = 0;
                            }
                        }
                    }
                    if (current_length == 255 || (length < current_length)) {
                        length = current_length;
                        if (search_buffer_index <= right_index) {
                            offset = right_index - search_buffer_index + 1;
                        } else {
                            offset = buffer_capacity - search_buffer_index + right_index + 1;
                        }
                        if (current_length == 255) {
                            break;
                        }
                    }
                } while (search_buffer_index != left_index);
            } else {
                look_ahead_buffer[look_ahead_buffer_length++] = static_cast<unsigned char>(input_file.get());
            }
            // write encode to file
            if (length == 0) {
                output_file << length << look_ahead_buffer[0];
            } else {
                output_file << offset << length;
                if (length < look_ahead_buffer_length) {
                    output_file << look_ahead_buffer[look_ahead_buffer_length - 1];
                } else {
                    break;
                }
            }
            // rewrite search buffer
            is_search_buffer_empty = false;
            if (length < buffer_capacity - 1) {
                for (int i = 0; i < look_ahead_buffer_length; ++i) {
                    if (!is_search_buffer_full && ((right_index + i + 1) == (buffer_capacity - 1))) {
                        is_search_buffer_full = true;
                    }
                    search_buffer[(right_index + i + 1) % buffer_capacity] = look_ahead_buffer[i];
                }
                if (is_search_buffer_full) {
                    right_index = (right_index + static_cast<int>(look_ahead_buffer_length)) % buffer_capacity;
                    left_index = (right_index + 1) % buffer_capacity;
                } else {
                    right_index = right_index + static_cast<int>(look_ahead_buffer_length);
                }
            } else {
                is_search_buffer_full = true;
                left_index = 0;
                right_index = buffer_capacity - 1;
                for (int i = left_index; i <= right_index; ++i) {
                    search_buffer[i] = look_ahead_buffer[length + 1 - buffer_capacity + i];
                }
            }
        }
    }
    input_file.close();
    output_file.close();
}

void lz77Decode(const char *input_file_name, const char *output_file_name) {
    std::ifstream input_file(input_file_name, std::ios::binary);
    std::ofstream output_file(output_file_name, std::ios::binary);
    const unsigned char buffer_capacity = 128;
    bool end_of_file = false;
    if (input_file.is_open() && output_file.is_open()) {
        unsigned char compression_methods = static_cast<unsigned char>(input_file.get());
        if (compression_methods != 2) {
            compression_methods &= ~(1 << 1);
            output_file << compression_methods;
        }
        unsigned char search_buffer[buffer_capacity];
        unsigned char additional_buffer[buffer_capacity];
        int left_index = 0;
        int right_index = 0;
        unsigned char offset = 0;
        unsigned char length = 0;
        unsigned char next = 0;
        bool is_search_buffer_full = false;
        while (input_file.peek() != EOF) {
            offset = static_cast<unsigned char>(input_file.get());
            if (offset == 0) {
                length = 0;
                next = static_cast<unsigned char>(input_file.get());
            } else {
                length = static_cast<unsigned char>(input_file.get());
                if (input_file.peek() != EOF) {
                    next = static_cast<unsigned char>(input_file.get());
                } else {
                    end_of_file = true;
                }
            }
            unsigned char current_length = length;
            unsigned char current_index =
                (right_index + 1 - offset) >= 0 ? right_index + 1 - offset : buffer_capacity + right_index + 1 - offset;
            int iteration = 0;
            unsigned char additional_buffer_length = 0;
            while (current_length > 0) {
                output_file << search_buffer[current_index];
                if (iteration >= (static_cast<int>(length) + 1 - static_cast<int>(buffer_capacity))) {
                    additional_buffer[additional_buffer_length] = search_buffer[current_index];
                    ++additional_buffer_length;
                }
                ++current_index;
                if (is_search_buffer_full && ((current_index) % buffer_capacity == left_index) ||
                    !is_search_buffer_full && (current_index > right_index)) {
                    current_index = (right_index + 1 - offset) >= 0 ? right_index + 1 - offset
                                                                    : buffer_capacity + right_index + 1 - offset;
                } else if (current_index == buffer_capacity) {
                    current_index = 0;
                }
                ++iteration;
                --current_length;
            }
            if (!end_of_file) {
                output_file << next;
                additional_buffer[additional_buffer_length] = next;
                ++additional_buffer_length;
            } else {
                break;
            }
            if (length < buffer_capacity - 1) {
                for (int i = 0; i < additional_buffer_length; ++i) {
                    if (!is_search_buffer_full && ((right_index + i + 1) == (buffer_capacity - 1))) {
                        is_search_buffer_full = true;
                    }
                    search_buffer[(right_index + i + 1) % buffer_capacity] = additional_buffer[i];
                }
                if (is_search_buffer_full) {
                    right_index = (right_index + static_cast<int>(additional_buffer_length)) % buffer_capacity;
                    left_index = (right_index + 1) % buffer_capacity;
                } else {
                    right_index = right_index + static_cast<int>(additional_buffer_length);
                }
            } else {
                is_search_buffer_full = true;
                left_index = 0;
                right_index = buffer_capacity - 1;
                for (int i = left_index; i <= right_index; ++i) {
                    search_buffer[i] = additional_buffer[i];
                }
            }
        }
    }
    input_file.close();
    output_file.close();
}