
#include "utils/utils.h"

int abs_int(int x) 
{
    return x < 0 ? -x : x;
}

int char_to_promotion_flag(char c)
{   
    if (c >= 'a' && c <= 'z')
        c -= 32; // 'n' -> 'N', etc.

    switch(c)
    {
        case 'N': return KNIGHT_PROMOTION;
        case 'B': return BISHOP_PROMOTION;
        case 'R': return ROOK_PROMOTION;
        case 'Q': return QUEEN_PROMOTION;
        default:
            return QUEEN_PROMOTION; // default to queen if invalid input
    } 
}

int is_pawn_promotion(Position* pos, Move* m)
{
    int bb_i = get_bb_index(pos->bb, m->start);
    if ((bb_i == WHITE_PAWN && m->dest <= 7) || 
        (bb_i == BLACK_PAWN && m->dest >= 56))
        return 1;
    return 0;
}

void choose_promotion_move(Move* m)
{
    printf("Chose promotion piece (N/B/R/Q): ");
    char c = getchar();
    while (getchar() != '\n' && !feof(stdin));

    m->flags = char_to_promotion_flag(c);
}

int square_string_to_int(char* square_string)
{
    if (!square_string) 
        return -1;

    if (square_string[0] < 'a' || square_string[0] > 'h')
        return -1;

    if (square_string[1] < '1' || square_string[1] > '8')
        return -1;

    int col = square_string[0] - 'a';
    int row = 8 - (square_string[1] - '0');
    return row * 8 + col;
}

const char* square_to_notation(int sq) 
{
    static char buff[8][3];   // allows up to 8 simultaneous results 
    static int idx = 0;
    char *out = buff[(idx++) & 7];
    if (sq < 0 || sq > 63) 
    {
        out[0] = out[1] = '?';
        out[2] = '\0';
        return out;
    }
    out[0] = 'a' + (sq % 8);
    out[1] = '8' - (sq / 8);
    out[2] = '\0';
    return out;
}

