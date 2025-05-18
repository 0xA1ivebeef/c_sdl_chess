
#include "utils/char_utils.h"

int char_to_int(const char c)
{
    switch(c)
    {
        case 'p':
            return 0;
        case 'n':
            return 1;
        case 'b':
            return 2;
        case 'r':
            return 3;
        case 'q':
            return 4;
        case 'k':
            return 5;
    }
    return 0;
}

char to_lower(char c)
{
    if(c > 64 && c < 91)
        return (char)(c + 32);
    return c;
}

int is_upper(const char c)
{
    return (c > 64 && c < 91);
}

int is_digit(const char c)
{
    return (c > 47 && c < 58);
}

int is_letter(const char c)
{
    return ((c > 64 && c < 91) || (c > 96 && c < 123));
}

