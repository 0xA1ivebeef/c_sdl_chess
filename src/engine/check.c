
#include "engine/check.h"

int is_check(int king_square, uint64_t attack_bitboard)
{
    // return if attack bitboard has set bit on own king square
    return (attack_bitboard & (1ULL << king_square)) != 0;
}

void filter_moves(Position* position)
{
    // for each pseudo legal move make it on the board
    // see if it puts current_player in check
    // add it if its does not
    Move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int valid_move_count = 0;

    // its white to move, get in pseudo legal moves and filter if white puts 
    // himself into check with any moves
    for (int i = 0; i < position->legal_move_count; i++)
    {
        Undo undo = {0};  
        Move m = position->legal_moves[i];

        // !!! apply move switches current player (now its black to move)
        apply_move(position, &m, &undo);  // save state in undo then apply move m to position

        // if its not check add the move
        if (!square_under_attack(position->bitboards, position->occupancy, position->king_square[!position->current_player], position->current_player))
            valid_moves[valid_move_count++] = m;

        undo_move(position, &m, &undo); // restore position
    }
    // update position legal moves to valid moves
    memcpy(position->legal_moves, valid_moves, sizeof(Move) * valid_move_count); // rest is discarded through legal_move_count
    position->legal_move_count = valid_move_count;
}

