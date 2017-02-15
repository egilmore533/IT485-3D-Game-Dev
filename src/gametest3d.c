/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "input.h"
#include "camera.h"

#define WINDOW_WIDTH			1024.0f
#define WINDOW_HEIGHT			768.0f
#define ASPECT_RATIO			WINDOW_WIDTH / WINDOW_HEIGHT
#define NEAR_CLIPPING_PLANE		0.1f
#define FAR_CLIPPING_PLANE		100.0f

static Uint8	gameLoop = 1;
static Command	*quitCommand = NULL;
static Command	*forwardCommand = NULL;
static Command	*backwardCommand = NULL;
static Command	*rightCommand = NULL;
static Command	*leftCommand = NULL;

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

static void quit();

char get_game_loop_status();

void initialize_systems();

void initialize_command();

int main(int argc, char *argv[])
{
	Camera *camera;
	glm::vec3 cameraPosition = glm::vec3(4.0f, 3.0f, 3.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::mat4 view = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f,1.0f,0.0f));
	//glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
	glm::mat4 model = glm::mat4(1.0f);
	GLuint model_location, view_location, projection_location;
	//GLuint MatrixID; 
	//glm::mat4 model_view_projection = projectionMatrix * view * model;

	GLuint vao; //vao == vertex array object
	GLuint cubeBufferObject;
	GLuint colorbuffer;

    const float triangleVertices[] = {
        0.0f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.366f, 0.0f, 1.0f,
        -0.5f, -0.366f, 0.0f, 1.0f,
        //next part contains vertex colors
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f  
    }; //we love you vertices!
    
    initialize_systems();
	camera = camera_new(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT),cameraPosition);
	initialize_command();


	model_location = glGetUniformLocation(graphics3d_get_shader_program(), "model");
	view_location = glGetUniformLocation(graphics3d_get_shader_program(), "view");
	projection_location = glGetUniformLocation(graphics3d_get_shader_program(), "projection");
	
	//MatrixID = glGetUniformLocation(graphics3d_get_shader_program(), "model_view_projection"); //this needs to match the uniform value in the shader file
        
        
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); //make our vertex array object, we need it to restore state we set after binding it. Re-binding reloads the state associated with it.
    
    glGenBuffers(1, &cubeBufferObject); //create the buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeBufferObject); //we're "using" this one now
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); //formatting the data for the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind any buffers

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    slog("glError: %d", glGetError());
    
    while (get_game_loop_status())
    {
		get_all_events();

		//model_view_projection = projectionMatrix * view * model;

        glClearColor(0.0,0.0,0.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* drawing code in here! */
		glUseProgram(graphics3d_get_shader_program());
        //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &model_view_projection[0][0]);
		glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(view_location, 1, GL_FALSE, &get_camera_view_matrix(camera)[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &get_camera_projection_matrix(camera)[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cubeBufferObject);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12*3);

		glDisableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		/*
		glBindBuffer(GL_ARRAY_BUFFER, cubeBufferObject); //bind the buffer we're applying attributes to
        glEnableVertexAttribArray(0); //0 is our index, refer to "location = 0" in the vertex shader
        glEnableVertexAttribArray(1); //attribute 1 is for vertex color data
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); //tell gl (shader!) how to interpret our vertex data
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float))); //color data is 48 bytes in to the array
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);

		*/
        /* drawing code above here! */
        graphics3d_next_frame();
    } 
    return 0;
}

static void quit()
{
	gameLoop = 0;
	return;
}

char get_game_loop_status()
{
	return gameLoop;
}

void initialize_systems()
{
	init_logger("gametest3d.log");
	if (graphics3d_init(WINDOW_WIDTH,WINDOW_HEIGHT,1,"gametest3d",33) != 0)
    {
        exit(-1);
    }
	command_initialize_system(64);
	camera_initialize_system(1);
}

void initialize_command()
{
	quitCommand = command_new(&quit, SDLK_ESCAPE);
	forwardCommand = command_new(&camera_translate_forward, SDLK_w);
	backwardCommand = command_new(&camera_translate_backward, SDLK_s);
	rightCommand = command_new(&camera_translate_backward, SDLK_d);
	leftCommand = command_new(&camera_translate_backward, SDLK_a);
}

/*eol@eof*/