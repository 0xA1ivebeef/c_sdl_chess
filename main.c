
#include "include/main/main.h"

int main()
{
    AppContext app = {0};
    if (init_sdl(&app) != 0) return 1;

    GameContext game = {0};
    if (setup(&game, app.renderer) != 0) return 1;

    game.running = 1;
    game_loop(&game);
 
    cleanup(&app);
    return 0;
}

