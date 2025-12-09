

#include "engine/fen_string_loader.h"

#define FENSTRING 1
#define fen_string fen_strings[FENSTRING]

static const char *fen_strings[] = 
{
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 
    "rn2k1r1/ppp1pp1p/3p2p1/5bn1/P7/2N2B2/1PPPPP2/2BNK1RR w kq - 4 11",
};

void substr(char* buffer, int start, int buffer_size)
{
    int i = 0;
    while(i < buffer_size - 1)
    {
        buffer[i] = fen_string[start];
        ++start;
        ++i;
    }
    buffer[buffer_size - 1] = '\0';
}

int get_castle_rights(char* token)
{
    int val = 0;
    for(size_t i = 0; i < strlen(token); ++i)
    {
        switch(token[i])
        {
            case 'K':
                val |= (1ULL << 3);
                break;
            case 'Q':
                val |= (1ULL << 2);
                break;
            case 'k':
                val |= (1ULL << 1);
                break;
            case 'q':
                val |= (1ULL << 0);
                break;
            default:
                break;
        }
    }
    return val;
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

int resolve_enpassant(char* token)
{
    if (strcmp(token, "-") == 0) 
        return -1;
    return square_string_to_int(token);
}

// TODO: hier eventuell mit raw pointer arbeiten und als array betrachten weil c cool ist
// allgemein logik optimieren, switch mit index wahrscheinlich nicht noetig
void resolve_token(Position* position, char* token, int index)
{
    // ! token can be '-' for enpassant flag, castle rights 
    switch(index)
    {
        case 0:
            position->current_player = !strcmp(token, "w");
            break;
        case 1:
            position->castle_rights = get_castle_rights(token);
            break;
        case 2: 
            position->enpassant_square = resolve_enpassant(token);
            break;
        case 3:
            position->halfmove_clock = 0;
            position->halfmove_clock = atoi(token);
            break;
        case 4:
            if (atoi(token))
                position->fullmove_number = atoi(token);
            else
                position->fullmove_number = 1;
            break;
        default:
            printf("undefined behaviour token resolve fen string\n");
            break;
    }
}

void resolve_game_flags(Position* position, int fen_string_index)
{
    int buffer_size = strlen(fen_string) - fen_string_index + 1;
    char buff[buffer_size];
    substr(buff, fen_string_index, buffer_size);
    int index = 0;
    char* token = strtok(buff, " ");
    while(token != NULL)
    {
        resolve_token(position, token, index);
        ++index;
        token = strtok(NULL, " ");
    }
}

void load_fen_string(Position* position)
{
    size_t i = 0;
    int bb_index;
    int col = 0;
    int row = 0;
    char c = fen_string[i];
    while((i < strlen(fen_string)) && c != ' ')
    {
        c = fen_string[i];
        if(c == '/')
        {
            row++;
            col = 0;
        }
        else if(is_digit(c))
        {
            col += (c - '0');
        }
        else if(is_letter(c))
        {
            bb_index = (char_to_int(to_lower(c)) + (is_upper(c) ? 6 : 0));
            position->bitboards[bb_index] |= (1ULL << (row * 8 + col));
            col++;
        }
        ++i;
    }

    resolve_game_flags(position, i);
}

