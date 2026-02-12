
#ifndef SDL_EVENT_HANDLER_H
#define SDL_EVENT_HANDLER_H

#include <SDL2/SDL.h>

#include "ui/ui_context.h"
#include "ui/mouse_event_handler.h"
#include "ui/renderer.h"

#include "engine/types.h"
#include "engine/move_handler.h"

int handle_event(AppContext* app, Position* pos, UIContext* ui, SDL_Event* event);

#endif
