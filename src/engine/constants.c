
#include "engine/constants.h"

#define FILE_A 0x0101010101010101ULL
#define FILE_H 0x8080808080808080ULL

const int file_offsets[8] = { 1, 0, -1,  0, 1, -1,  1, -1 };
const int rank_offsets[8] = { 0, 1,  0, -1, 1,  1, -1, -1 };

const int translation_table[5] = {1, 3, 3, 5, 9};
const int depth = 3;

