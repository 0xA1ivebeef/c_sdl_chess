
#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>

#include "engine/types.h"
#include "engine/constants.h"
#include "engine/check.h"
#include "engine/move_generator.h"
#include "engine/transposition_table.h"

#include "utils/utils.h"

int count_material(uint64_t* bb, uint8_t player);
int evaluate(Position* pos);
Move search_root(Position* pos, int depth);
void search_test(Position* pos);

#endif
