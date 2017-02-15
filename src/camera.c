#include <SDL.h>

#include <glm\gtx\transform.hpp>

#include "simple_logger.h"
#include "graphics3d.h"
#include "camera.h"

static int cameraNum = 0;
static int cameraMax = 0;
static Camera *cameraList = NULL;

/**
 * @brief frees the cameraList by freeing every camera in it and resetting the cameraMax
 * @param camera double pointer to the camera that needs to be freed
 */
static void camera_close_system()
{
	int i;
	Camera *camera;
	if(!cameraList)
	{
		slog("cameraList not initialized");
		return;
	}
	for(i = 0; i < cameraMax; i++)
	{
		camera = &cameraList[i];
		camera_free(&camera);
	}
	cameraNum = 0;
	free(cameraList);
	cameraList = NULL;
}

/**
 * @brief intializes the camera system for the number of cameras needed
 * @param camera_num the number of cameras that need to be created
 */
void camera_initialize_system(int camera_num)
{
	int i;
	if(camera_num == 0)
	{
		slog("camera_num == 0");
		return;
	}
	cameraList = (Camera *) malloc (sizeof (Camera) * camera_num);
	if(!cameraList)
	{
		slog("failed to initialize cameraList");
		return;
	}
	memset(cameraList, 0, sizeof (Camera) * camera_num);
	cameraNum = 0;
	
	/* If I add pointers to the camera entity make sure to add null declarations here
	for(i = 0; i < camera_num; i++)
	{
		
	}
	*/
	cameraMax = camera_num;
	atexit(camera_close_system);
}

/**
 * @brief frees the camera from memory
 * @param camera double pointer to the camera that needs to be freed
 */
void camera_free(Camera **camera)
{
	Camera *self;
	if(!camera)
	{
		return;
	}
	else if(!*camera) 
	{
		return;
	}
	self = *camera;
	self->active--;

	cameraNum--;
	*camera = NULL;
}

/**
 * @brief creates a new camera with the given resolution
 * @param screen_size the size the camera is being given for drawing
 * @param position the point the camera is in game space
 * @return the newly created camera
 */
Camera *camera_new(glm::vec2 screen_size, glm::vec3 position)
{
	int i;
	/*makesure we have the room for a new sprite*/
	if(cameraNum + 1 > cameraMax)
	{
		slog("Maximum cameras Reached.");
		exit(1);
	}
	for(i = 0; i < cameraMax; i++)
	{
		if(cameraList[i].active)
		{
			continue;
		}

		memset(&cameraList[i],0,sizeof(Camera));

		cameraList[i].active = 1;
		cameraList[i].position = position;
		cameraList[i].target = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraList[i].forward = glm::vec3(0.0f, 0.0f, -1.0f); 
		cameraList[i].up = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraList[i].right = glm::normalize(glm::cross(cameraList[i].forward,cameraList[i].up));

		cameraList[i].view_matrix = glm::lookAt(position, cameraList[i].target, cameraList[i].up);

		cameraList[i].projection_matrix = glm::perspective(glm::radians(45.0f), screen_size.x / screen_size.y, NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);

		cameraList[i].pitch = 0.0f;
		cameraList[i].yaw = -90.0f;
		cameraList[i].roll = 0.0f;

		cameraNum++;

		return &cameraList[i];
	}
	return NULL;
}

/**
 * @brief getter for a camera's view matrix
 * @param camera pointer to the camera whose view is wanted
 * @return a camera's view matrix
 */
glm::mat4 get_camera_view_matrix(Camera *camera)
{
	glm::mat4 view = camera->view_matrix;
	return view;
}

/**
 * @brief getter for a camera's projection matrix
 * @param camera pointer to the camera whose projection is wanted
 * @return a camera's projection matrix
 */
glm::mat4 get_camera_projection_matrix(Camera *camera)
{
	glm::mat4 projection = camera->projection_matrix;
	return projection;
}

static void camera_translate(Camera *camera, glm::vec3 direction, Uint8 pos)
{
	Uint32 time = get_game_time();
	float cam_speed = 5.0f * time / 1000;

	if(pos)
		cameraList[0].position += cam_speed * cameraList[0].forward;
	else
		cameraList[0].position -= cam_speed * cameraList[0].forward;
}

/**
 * @brief translates the camera forward by 1
 */
void camera_translate_forward()
{
	camera_translate(&cameraList[0], cameraList[0].forward, 1);
}

/**
 * @brief translates the camera backward by 1
 */
void camera_translate_backward()
{
	camera_translate(&cameraList[0], cameraList[0].forward, 0);
}

/**
 * @brief translates the camera left by 1
 */
void camera_translate_left()
{
	camera_translate(&cameraList[0], cameraList[0].right, 0);
}

/**
 * @brief translates the camera right by 1
 */
void camera_translate_right()
{
	camera_translate(&cameraList[0], cameraList[0].forward, 1);
}

/**
 * @brief rotates the camera right by 1 degree
 */
void camera_rotate_right();

/**
 * @brief rotates the camera left by 1 degree
 */
void camera_rotate_left();

/**
 * @brief gets a camera from the camrea list based on the cameras number
 * @param camera_num the camera to get, can be the player number (PLAYER_1,2,3,4)
 * @return the camera at that position in the camera list
 */
Camera *get_camera_by_num(int camera_num)
{
	return &cameraList[camera_num];
}
