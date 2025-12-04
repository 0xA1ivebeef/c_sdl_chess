
#ifndef UI_CONTEXT_H
#define UI_CONTEXT_H

typedef struct 
{
    int running;
    int selected_square;
    int needs_update;
    int game_over;
} UIContext;

#endif 
