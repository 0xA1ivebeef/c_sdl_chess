
#ifndef SDL_EVENT_HANDLER_H
#define SDL_EVENT_HANDLER_H

#include <SDL2/SDL.h>

#include "ui/mouse_event_handler.h"
#include "ui/renderer.h"

#include "engine/game_context.h"
#include "engine/move_handler.h"


void handle_event(GameContext* game, SDL_Event* event);

#endif
