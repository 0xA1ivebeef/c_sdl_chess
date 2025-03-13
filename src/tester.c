
#include "include/tester.h"

int get_nodes(uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, int depth)
{
    if(depth == 0)
        return 1;

    move current_moves[LEGAL_MOVES_SIZE] = {0};
    generate_legal_moves(game_flags[0], bitboards, occupancy_bitboards, game_flags, current_moves, get_attack_bitboard(!game_flags[0], bitboards, occupancy_bitboards, game_flags));
    printf("depth: %d\n legal_moves:\n", depth);
    log_legal_moves(current_moves);

    int num_positions = 0;
    uint64_t copy_bb[12] = {0};
    uint64_t copy_occ[3] = {0};
    int endindex = get_endindex(current_moves);
    for(int i = 0; i < endindex; ++i)
    {
        get_bb_copy(bitboards, occupancy_bitboards, copy_bb, copy_occ);
        apply_move(copy_bb, current_moves[i].startsquare, current_moves[i].destsquare);
        printf("applying move: %d, %d to bitboard: \n", current_moves[i].startsquare, current_moves[i].destsquare);
        num_positions += get_nodes(copy_bb, copy_occ, game_flags, depth-1);
    }
    return num_positions;
}
