
#include "utils/utils.h"

double get_time_seconds()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int enpassant_legal(Position* pos, Move m, Undo* undo)
{
    if (pos->enpassant != INVALID_SQUARE && is_double_pawn_move(m, undo->moved_piece))
    {
        if (undo->moved_piece % 6 == 0)
        {
            if (((pos->enpassant % 8 != 0) && (get_bb_index(pos->bb, move_to(m) - 1) == ((undo->moved_piece + 6) % 12))) || 
                ((pos->enpassant % 8 != 7) && (get_bb_index(pos->bb, move_to(m) + 1) == ((undo->moved_piece + 6) % 12))))
                return 1;
        }
    }
    return 0;
}

int get_king_sq(Position* pos, uint8_t player)
{
    uint64_t bb = pos->bb[player ? WHITE_KING : BLACK_KING];
    return bb ? __builtin_ctzll(bb) : -1;
}

int get_bb_index(uint64_t* bb, int sq)
{
    for(int i = 0; i < 12; ++i)
    {
        if(bb[i] & (1ULL << sq))
            return i;
    }
    return -1;
}

void generate_occ(Position* pos)
{
    for(int i = 0; i < 3; ++i)
        pos->occ[i] = 0;
    
    for(int i = 0; i < 6; ++i)
        pos->occ[BLACK] |= pos->bb[i];
 
    for(int i = 6; i < 12; ++i)
        pos->occ[WHITE] |= pos->bb[i];
    
    pos->occ[2] |= pos->occ[BLACK] | pos->occ[WHITE];
}

uint8_t get_legal_move_count(Move* lm)
{
	int i = 0;
	while(lm[i] != 0)
		++i;
	return i;
}

int abs_int(int x) 
{
    return x < 0 ? -x : x;
}

uint16_t char_to_promotion_flag(char c)
{   
    if (c >= 'a' && c <= 'z')
        c -= 32; // 'n' -> 'N', etc.

    switch(c)
    {
        case 'N': return PROMOTION_FLAGS[0];
        case 'B': return PROMOTION_FLAGS[1];
        case 'R': return PROMOTION_FLAGS[2];
        case 'Q': return PROMOTION_FLAGS[3];
        default:
            return PROMOTION_FLAGS[3]; // default to queen if invalid input
    } 
}

int is_pawn_promotion(Position* pos, Move m)
{
    int moved_piece = get_bb_index(pos->bb, move_from(m));
    if ((moved_piece == WHITE_PAWN && move_to(m) < 8) || 
        (moved_piece == BLACK_PAWN && move_to(m) > 55))
        return 1;
    return 0;
}

void choose_promotion_move(Move* m)
{
    printf("Chose promotion piece (N/B/R/Q): ");
    char c = getchar();
    while (getchar() != '\n' && !feof(stdin));

    *m |= char_to_promotion_flag(c);
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

int is_enpassant_move(uint8_t enpassant, Move m, int moved_piece)
{
    if (moved_piece != WHITE_PAWN && moved_piece != BLACK_PAWN)
        return 0;

    return enpassant == move_to(m);
}

const uint16_t CASTLING_MOVES[4] = 
{
    // 0000 startsquare destsquare
    (60 << 6) | 62, // white ks
    (60 << 6) | 58, // white qs
    ( 4 << 6) |  6, // black ks
    ( 4 << 6) |  2  // black qs
};

int is_castling_move(Move m, int moved_piece)
{
    // when handle special move is called moved piece already moved
    if (moved_piece != WHITE_KING && moved_piece != BLACK_KING)
        return 0;

    for (int i = 0; i < 4; ++i)
        if (m == CASTLING_MOVES[i])
            return 1;
    return 0;
}

int is_promotion_move(Move m)
{
    return (m >> 12) != 0;
}

int is_double_pawn_move(Move m, int moved_piece)
{
    if (moved_piece != WHITE_PAWN && moved_piece != BLACK_PAWN)
        return 0;

    return abs_int(move_from(m) - move_to(m)) == 16;
}

