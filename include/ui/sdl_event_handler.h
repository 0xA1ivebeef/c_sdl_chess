
#ifndef SDL_EVENT_HANDLER_H
#define SDL_EVENT_HANDLER_H

#include "engine/types.h"
#include "engine/move_handler.h"

#include "ui/sdl_wrapper.h"
#include "ui/renderer.h"

int handle_event(AppContext* app, Position* pos, UIContext* ui, SDL_Event* event);

#endif
