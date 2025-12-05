
#include "utils/utils.h"

const char* square_to_notation(int sq) 
{
    static char bufs[8][3];   // allows up to 8 simultaneous results 
    static int idx = 0;
    char *out = bufs[(idx++) & 7];

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

