
#ifndef ATTACK_GENERATOR_H
#define ATTACK_GENERATOR_H

#include "ui/console_logger.h"

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/bitmask_loader.h"

uint64_t get_attack_bb(Position* pos);
void generate_attack_bb(Position* position);

#endif
