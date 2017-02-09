#include <SDL.h>
#include <stdio.h>
#include "simple_logger.h"
#include "input.h"

static Command	*commandList = NULL;
static int		commandNum = 0;
static int		commandMax = 0;


Command *command_new(void (*command_to_execute)(), SDL_Keycode key_bind)
{
	int i;
	/*makesure we have the room for a new sprite*/
	if(commandNum + 1 > commandMax)
	{
		slog("Maximum commands Reached.");
		exit(1);
	}
	for(i = 0; i < commandMax; i++)
	{
		if(commandList[i].used)
		{
			continue;
		}

		memset(&commandList[i],0,sizeof(Command));

		commandList[i].used = 1;
		commandList[i].command_to_execute = command_to_execute;
		commandNum++;

		return &commandList[i];
	}
	return NULL;
}

static void command_close_system()
{
	int i;
	Command *command;
	if(!commandList)
	{
		slog("commandList not initialized");
		return;
	}
	for(i = 0; i < commandMax; i++)
	{
		command = &commandList[i];
		command_free(&command);
	}
	commandNum = 0;
	free(commandList);
	commandList = NULL;
}

void command_initialize_system(int maxCommands)
{
	int i;
	if(maxCommands == 0)
	{
		slog("Max commands == 0");
		return;
	}
	commandList = (Command *) malloc (sizeof (Command) * maxCommands);
	if(!commandList)
	{
		slog("failed to initialize commandList");
		return;
	}
	memset(commandList, 0, sizeof (Command) * maxCommands);
	commandNum = 0;
	for(i = 0; i < maxCommands; i++)
	{
		commandList[i].command_to_execute = NULL;
		commandList[i].bound_key = NULL;
	}
	commandMax = maxCommands;
	atexit(command_close_system);
}

void command_free(Command **command)
{
	Command *self;
	if(!command)
	{
		return;
	}
	else if(!*command) 
	{
		return;
	}
	self = *command;
	self->used--;

	commandNum--;
	*command = NULL;
}

void get_all_events()
{
	SDL_Event sdl_event;
	if ( SDL_PollEvent(&sdl_event) ) 
    {
		handle_input_event(sdl_event);
	}
}

void handle_input_event(SDL_Event sdl_event)
{
	switch(sdl_event.type)
	{
		case SDL_KEYDOWN:
			slog("key pressed");
			execute_all_bound(sdl_event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			break;
	}
	return;
}

void execute_all_bound(SDL_Keycode key_pressed)
{
	int i;
	slog("%d",commandMax);
	for(i = commandMax; i > 0; i--)
	{
		slog("%d",commandList[i].used);
		if(!commandList[i].used)
		{
			continue;
		}
		if(commandList[i].bound_key != key_pressed)
		{
			continue;
		}

		commandList[i].command_to_execute();
	}
}

