#ifndef COMPRESS_LZ_77
#define COMPRESS_LZ_77

#include <fstream>

void lz77Encode(const char *input_file_name, const char *output_file_name, bool first_method = true);

void lz77Decode(const char *input_file_name, const char *output_file_name);

#endif