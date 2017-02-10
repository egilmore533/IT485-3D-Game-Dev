#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL.h>
#include <glm\glm.hpp>
#include "graphics3d.h"

#define MAX_SPRITE_FILENAME		512

typedef struct
{
	SDL_Surface		*image;
	GLuint			texture;
	char			filename[MAX_SPRITE_FILENAME];
	Uint32			width, height;
	Uint32			framesperline;
	Uint8			used; 
}Sprite;

void sprite_intialize_system(int maxSprites);

void sprite_free(Sprite **sprite);

Sprite *sprite_new(char *filename, glm::vec2 frameSize);

void sprite_draw(Sprite *sprite, SDL_Surface *surface, glm::vec2 size, int frame);

#endif
