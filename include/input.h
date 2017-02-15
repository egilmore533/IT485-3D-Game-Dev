#ifndef __INPUT_H__
#define __INPUT_H__

#include "camera.h"

typedef struct
{
	Uint32 used;								/**< in use flag */
	SDL_Keycode bound_key;						/**< the key that has been bound to this command */
	void (*command_to_execute)();				/**< the code to be executed in this command, this will take an entity eventually */
}Command;

Command *command_new(void (*command_to_execute)(), SDL_Keycode key_bind);

void command_free(Command **command);

void command_initialize_system(int maxCommands);

void execute_all_bound(SDL_Keycode key_pressed);

void handle_input_event(SDL_Event sdl_event);

void get_all_events();

#endif
