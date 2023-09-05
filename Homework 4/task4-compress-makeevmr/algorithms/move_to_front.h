#ifndef COMPRESS_MOVE_TO_FRONT_H
#define COMPRESS_MOVE_TO_FRONT_H

#include <fstream>

void fillCodes(unsigned char *codes);

unsigned char findAndMoveValue(unsigned char *codes, unsigned char symbol);

void moveValueFront(unsigned char *codes, int index);

void moveToFrontEncode(const char *input_file_name, const char *output_file_name);

void moveToFrontDecode(const char *input_file_name, const char *output_file_name);

#endif // !COMPRESS_MOVE_TO_FRONT_H
