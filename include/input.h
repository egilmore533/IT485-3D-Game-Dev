#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL.h>
#include "camera.h"

typedef struct
{
	Uint32 used;								/**< in use flag */
	SDL_Keycode bound_key;						/**< the key that has been bound to this command */
	void (*command_to_execute)();				/**< the code to be executed in this command, this will take an entity eventually */
	
	void (*motion_command_to_execute)();		/**< motion events need the data so pass it into a different execution function pointer */
}Command;

Command *command_new(SDL_Keycode key_bind, void (*command_to_execute)(), void (*motion_command_to_execute)());

void command_free(Command **command);

void command_initialize_system(int maxCommands);

void execute_all_bound(SDL_Keycode key_pressed);

void handle_input_event(SDL_Event sdl_event);

void execute_motion_commands();

void get_all_events();

void execute_motion_commands();

#endif
