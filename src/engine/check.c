
#include "engine/check.h"

int is_check(int king_sq, uint64_t attack_bb)
{
    // return if attack bitboard has set bit on own king square
    return 0;

    // return ((attack_bb & (1ULL << king_sq)) != 0);
}

int square_under_attack()
{
    return 0;
}

void filter_moves(Position* pos)
{
    // for each pseudo legal move make it on the board
    // see if it puts current_player in check
    // add it if its does not
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;

    // its white to move, get in pseudo legal moves and filter if white puts 
    // himself into check with any moves
    for (int i = 0; i < pos->legal_move_count; i++)
    {
        Undo undo = {0};  
        Move m = pos->legal_moves[i];

        // !!! apply move switches current player (now its black to move)
        apply_move(pos, &m, &undo);  

        // if its not check add the move
        if (!square_under_attack())
            valid_moves[valid_move_count++] = m;

        undo_move(pos, &m, &undo); // restore position
    }
    // update position legal moves to valid moves
    memcpy(pos->legal_moves, valid_moves, sizeof(Move) * valid_move_count); 
    pos->legal_move_count = valid_move_count;
}

