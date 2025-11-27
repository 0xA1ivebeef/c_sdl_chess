
#ifndef SDL_EVENT_HANDLER_H
#define SDL_EVENT_HANDLER_H

#include <SDL2/SDL.h>

#include "ui/ui_context.h"
#include "ui/mouse_event_handler.h"
#include "ui/renderer.h"

#include "engine/move.h"
#include "engine/position.h"
#include "engine/move_handler.h"

int handle_event(Position* position, UIContext* ui_context, SDL_Event* event);

#endif
