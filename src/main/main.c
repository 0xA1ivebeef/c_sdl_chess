
#include "main/main.h"

int main()
{
    srand(time(NULL));  
    AppContext app = {0}; // sdl components
    if (init_sdl(&app) != 0) 
    {
        return 1;
    }

    if (load_renderer(app.renderer) != 0)
    {
        return 1;
    }

    Position position = {0}; 
    if (setup(&position) != 0) 
    {
        return 1;
    }

    UIContext ui_context = {0};
    ui_context.running = 1;
    ui_context.game_over = 0;

    game_loop(&position, &ui_context); // game.h
 
    cleanup(&app);
    return 0;
}

