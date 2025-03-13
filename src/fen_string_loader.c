
#include "include/fen_string_loader.h"

static const char* fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
// "rn2k1r1/ppp1pp1p/3p2p1/5bn1/P7/2N2B2/1PPPPP2/2BNK1RR w kq - 4 11";

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
    if(square_string[0] == '-')
        return -1;

    int col = square_string[0] - 'a';
    int row = 8 - (square_string[1] - '0');
    return row * 8 + col;
    
}

void resolve_token(char* token, int index, int* game_flags)
{
    switch(index)
    {
        case 0:
            game_flags[0] = !strcmp(token, "w");
            break;
        case 1:
            game_flags[1] = get_castle_rights(token);
            break;
        case 2:
            game_flags[2] = square_string_to_int(token);
            break;
        case 3:
            game_flags[3] = atoi(token);
            break;
        case 4:
            game_flags[4] = atoi(token);
            break;
        default:
            printf("undefined behaviour token resolve fen string\n");
            break;
    }
}

void resolve_game_flags(int* game_flags, int fen_string_index)
{
    int buffer_size = strlen(fen_string) - fen_string_index + 1;
    char buff[buffer_size];
    substr(buff, fen_string_index, buffer_size);
    int index = 0;
    char* token = strtok(buff, " ");
    while(token != NULL)
    {
        if(token[0] != '-')
            resolve_token(token, index, game_flags);
        ++index;
        token = strtok(NULL, " ");
    }
}

void load_fen_string(uint64_t* bitboards, int* game_flags)
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
            bitboards[bb_index] |= (1ULL << (row * 8 + col));
            col++;
        }
        ++i;
    }

    resolve_game_flags(game_flags, i);
}

