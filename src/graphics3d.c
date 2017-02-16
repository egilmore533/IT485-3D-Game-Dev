#include <math.h>
#include <stdio.h>

#include "graphics3d.h"
#include "simple_logger.h"
#include "shader.h"

static SDL_GLContext		__graphics3d_gl_context;
static SDL_Window			*__graphics3d_window = NULL;
static GLuint				__graphics3d_shader_program;

/* timing things */
static Uint32				graphicsFrameDelay = 33;
static Uint32				time = 0;
static Uint32				graphicsThen = 0;
static Uint8				graphicsPrintFPS = 0;
static float				graphicsFPS = 0; 

void graphics3d_close();

GLuint graphics3d_get_shader_program()
{
    return __graphics3d_shader_program;
}

void graphics3d_next_frame()
{
    Uint32 difference;
	graphicsThen = time;
	time = SDL_GetTicks();
	difference = (time - graphicsThen);
    SDL_GL_SwapWindow(__graphics3d_window);
    if (difference < graphicsFrameDelay)
    {
        SDL_Delay(graphicsFrameDelay - difference);        
    }
    graphicsFPS = 1000.0/MAX(SDL_GetTicks() - graphicsThen,0.001);
    if (graphicsPrintFPS)
    {
        printf("FPS: %f\n",graphicsFPS); //printf so the log isn't filled with frame rate
    }
}

int graphics3d_init(int sw,int sh,int fullscreen,const char *project,Uint32 frameDelay)
{
    const unsigned char *version;
    GLenum glew_status;
        
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        slog("failed to initialize SDL!");
        return -1;
    }
    atexit(SDL_Quit);
    graphicsFrameDelay = frameDelay;
    
    __graphics3d_window = SDL_CreateWindow(project?project:"gametest3d",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              sw, sh,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
	if(__graphics3d_window == NULL)
	{
		slog("FATAL: There was an error creating the SDL window!\n");
		return -1;
	
	}
    
    __graphics3d_gl_context = SDL_GL_CreateContext(__graphics3d_window);
    if (__graphics3d_gl_context == NULL)
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    version = glGetString(GL_VERSION);
    if (version == NULL) 
    {
        slog("There was an error creating the OpenGL version number!\n");
        return -1;
    }
    
	//if we wanted a minimap we would make another context here
    SDL_GL_MakeCurrent(__graphics3d_window, __graphics3d_gl_context);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    //MUST make a context AND make it current BEFORE glewInit()!
    glewExperimental = GL_TRUE;
    glew_status = glewInit();
    if (glew_status != 0) 
    {
        slog("Error: %s", glewGetErrorString(glew_status));
        return -1;
    }
    
	SDL_SetRelativeMouseMode(SDL_TRUE);
    
    __graphics3d_shader_program = BuildShaderProgram("shaders/vs1.glsl", "shaders/fs1.glsl");
    if (__graphics3d_shader_program == -1)
    {
        return -1;
    }
    
    slog("Using program %d", __graphics3d_shader_program);
        
    atexit(graphics3d_close);
    return 0;
}

void graphics3d_close()
{
    SDL_GL_DeleteContext(__graphics3d_gl_context);
}

void graphics3d_frame_begin()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glPushMatrix();
}

/**
 * @brief gets the game time
 * @return the current game time
 */
Uint32 get_game_time()
{
	return time;
}

/*eol@eof*/
