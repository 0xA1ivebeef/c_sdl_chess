
#include "main/main.h"

int main()
{
    srand(time(NULL));  

    AppContext app = {0}; 
    if (init_sdl(&app) != 0) 
        return -1;

    Position pos = {0}; 
    position_init(&pos);

    // running, selected_square, needs_update, game_over
    UIContext ui_context = { 1, -1, 1, 0 };
    game_loop(&app, &pos, &ui_context); 
 
    cleanup(&app);
    return 0;
}

