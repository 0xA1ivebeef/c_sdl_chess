
#ifndef SDL_EVENT_HANDLER_H
#define SDL_EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include "engine/game_context.h"

void handle_event(GameContext* game, SDL_Event* event);

#endif
