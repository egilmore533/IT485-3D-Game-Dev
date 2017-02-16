#include <stdio.h>
#include "simple_logger.h"
#include "input.h"
#include "graphics3d.h"

static Command	*commandList = NULL;
static int		commandNum = 0;
static int		commandMax = 0;

static Uint8	getInputDelay = 1;
static Uint32	inputTime = 0;
static Uint32	inputExecuted = 0;
static Uint32	inputDelay = 0;

void update_input_time();
void slog_input_delay();


Command *command_new(SDL_Keycode key_bind, void (*command_to_execute)(), void (*motion_command_to_execute)())
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
		commandList[i].bound_key = key_bind;
		commandList[i].command_to_execute = command_to_execute;
		commandList[i].motion_command_to_execute = motion_command_to_execute;
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
		commandList[i].motion_command_to_execute = NULL;
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
	update_input_time();
	switch(sdl_event.type)
	{
		case SDL_KEYDOWN:
			execute_all_bound(sdl_event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			break;
		case SDL_MOUSEMOTION:
			execute_motion_commands();
			break;
		case SDL_CONTROLLERAXISMOTION:
			execute_motion_commands();
			break;
			
	}
	return;
}

void execute_all_bound(SDL_Keycode key_pressed)
{
	int i;
	for(i = 0; i < commandMax; i++)
	{
		if(!commandList[i].used)
		{
			continue;
		}
		if(commandList[i].bound_key != key_pressed)
		{
			continue;
		}

		slog_input_delay();
		commandList[i].command_to_execute();
	}
}

void execute_motion_commands()
{
	int i;
	for(i = 0; i < commandMax; i++)
	{
		if(!commandList[i].used)
		{
			continue;
		}
		if(!commandList[i].motion_command_to_execute)
		{
			continue;
		}

		slog_input_delay();
		commandList[i].motion_command_to_execute();
	}
}

void update_input_time()
{
	if(getInputDelay)
	{
		inputTime = get_game_time();
	}
}

void slog_input_delay()
{
	if(getInputDelay)
	{
		inputExecuted = SDL_GetTicks();
		inputDelay = inputExecuted - inputTime;
		slog("Input Delay: %d Ticks",inputDelay);
	}
}
