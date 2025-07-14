
#include "include/main/main.h"

int main()
{
    AppContext app = {0}; // sdl components
    if (init_sdl(&app) != 0) 
        return 1;

    GameContext game = {0}; // bitboards, game_flags
    if (setup(&game, app.renderer) != 0) 
        return 1;

    game.running = 1;
    game_loop(&game); // game.h
 
    cleanup(&app);
    return 0;
}

