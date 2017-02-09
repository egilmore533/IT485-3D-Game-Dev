#ifndef __INPUT_H__
#define __INPUT_H__

typedef struct 
{
	Uint32 used;
	SDL_Keycode bound_key;
	void (*command_to_execute) ();
}Command;

Command *command_new(void (*command_to_execute)(), SDL_Keycode key_bind);

void command_free(Command **command);

void command_initialize_system(int maxCommands);

void execute_all_bound(SDL_Keycode key_pressed);

void handle_input_event(SDL_Event sdl_event);

void get_all_events();

char get_game_loop_status();

#endif
