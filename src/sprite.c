#include <SDL_image.h>

#include "sprite.h"
#include "simple_logger.h"
#include "graphics3d.h"

static Sprite	*spriteList = NULL;
static Uint32	spriteMax = 0;
static Uint32	spriteNum = 0;

Sprite *sprite_get_by_filename(char *filename)
{
    int i;
    for (i = 0;i < spriteMax;i++)
    {
        if  ((spriteList[i].used > 0)
            &&(strcmp(spriteList[i].filename,filename) == 0))
        {
            return &spriteList[i];
        }
    }
    return NULL;
}

Sprite *sprite_get_next_free()
{
	Uint32 i;
	for(i = 0; i < spriteMax; i++)
	{
		if(!spriteList[i].used)
		{
			memset(&spriteList[i],0,sizeof(Sprite));
			spriteList[i].used = 1;
			return &spriteList[i];
		}
	}
}

void sprite_system_close()
{
	int i;
	Sprite *sprite;
	if(!spriteList)
	{
		slog("spriteList not initialized");
		return;
	}
	for(i = 0; i < spriteMax; i++)
	{
		sprite = &spriteList[i];
		sprite_free(&sprite);
	}
	spriteNum = 0;
	free(spriteList);
	spriteList = NULL;
}

void sprite_free(Sprite **sprite)
{
	Sprite *self;
	if(!sprite)
	{
		return;
	}
	else if(!*sprite)
	{
		return;
	}
	self = *sprite;
	self->used--;
	if(self->image != NULL)
	{
		SDL_FreeSurface(self->image);
	}
	memset(sprite,0,sizeof(Sprite));
}

void sprite_intialize_system(int maxSprites)
{
	int i;
	if(maxSprites == 0)
	{
		slog("max Sprites == 0");
		return;
	}
	spriteList = (Sprite *) malloc (sizeof (Sprite) * maxSprites);
	if(!spriteList)
	{
		slog("failed to initialize spriteList");
		return;
	}
	memset(spriteList, 0, sizeof (Sprite) * maxSprites);
	spriteNum = 0;
	for(i = 0; i < maxSprites; i++)
	{
		spriteList[i].image = NULL;
	}
	spriteMax = maxSprites;
	atexit(sprite_system_close);
}

Sprite *sprite_new(char *filename, glm::vec2 frameSize)
{
	Sprite *sprite;
	SDL_Surface *image;
	int mode = GL_RGB;

	sprite = sprite_get_by_filename(filename);
	if(sprite)
	{
		return sprite;
	}

	image = IMG_Load(filename);
	if(!image)
	{
		slog("failed to load sprite image file: %s, error: %s", filename, SDL_GetError());
		return NULL;
	}

	sprite->image = image;

	//if no frameSize specified the frame size is the whole image
	if(frameSize.x == sprite->image->w) 
	{
		sprite->width = sprite->image->w;
	}
	if(frameSize.y == sprite->image->h) 
	{
		sprite->height = sprite->image->h;
	}

	glGenTextures(1, &sprite->texture);
    glBindTexture(GL_TEXTURE_2D, sprite->texture);

	if(sprite->image->format->BytesPerPixel == 4)
	{
        mode = GL_RGBA;
    }

	glTexImage2D(GL_TEXTURE_2D, 0, mode, sprite->image->w, sprite->image->h, 0, mode, GL_UNSIGNED_BYTE, sprite->image->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    return sprite;
}


