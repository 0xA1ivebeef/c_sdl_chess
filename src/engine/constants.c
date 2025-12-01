
#include "engine/constants.h"

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

const int file_offsets[8] = { 1, 0, -1,  0, 1, -1,  1, -1 };
const int rank_offsets[8] = { 0, 1,  0, -1, 1,  1, -1, -1 };

const int translation_table[5] = {1, 3, 3, 5, 9};
const int depth = 3;

