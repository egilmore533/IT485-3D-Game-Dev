#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm\glm.hpp>

/**
 * Manages the games cameras. The cameras use a perspective projection. 
 */

#define NEAR_CLIPPING_PLANE		0.1f
#define FAR_CLIPPING_PLANE		100.0f

enum Direction
{
	Forward,
	Backward,
	Right,
	Left
};


typedef struct
{
	int active;			/**< whether this camera is active or not */
	/**< owner entity would go here if I had entities already 
	     this would be how I would check to update the camera's position, rotation, etc. */

	glm::vec3 position;		/**< position of the camera in the game world */
	glm::vec3 target;		/**< the position the camera is looking to */
	glm::vec3 forward;		/**< the direction the camera is looking */
	glm::vec3 right;		/**< the direction right of the camera */
	glm::vec3 up;			/**< the direction above the camera */

	float yaw;		/**< x rotation */
	float pitch;	/**< y rotation */
	float roll;		/**< z rotation */

	glm::mat4 view_matrix;			
	glm::mat4 projection_matrix;

	int prev_mouse_x, prev_mouse_y;
}Camera;

/**
 * @brief intializes the camera system for the number of cameras needed
 * @param camera_num the number of cameras that need to be created
 */
void camera_initialize_system(int camera_num);

/**
 * @brief frees the camera from memory
 * @param camera double pointer to the camera that needs to be freed
 */
void camera_free(Camera **camera);

/**
 * @brief creates a new camera with the given resolution
 * @param screen_size the size the camera is being given for drawing
 * @param position the point the camera is in game space
 * @return the newly created camera
 */
Camera *camera_new(glm::vec2 screen_size, glm::vec3 position);

/**
 * @brief getter for a camera's view matrix
 * @param camera pointer to the camera whose view is wanted
 * @return a camera's view matrix
 */
glm::mat4 get_camera_view_matrix(Camera *camera);

/**
 * @brief getter for a camera's projection matrix
 * @param camera pointer to the camera whose projection is wanted
 * @return a camera's projection matrix
 */
glm::mat4 get_camera_projection_matrix(Camera *camera);

/**
 * @brief translates the camera forward by 1
 */
void camera_translate_forward();

/**
 * @brief translates the camera backward by 1
 */
void camera_translate_backward();

/**
 * @brief translates the camera left by 1
 */
void camera_translate_left();

/**
 * @brief translates the camera right by 1
 */
void camera_translate_right();

void camera_rotate();

/**
 * @brief gets a camera from the camrea list based on the cameras number
 * @param camera_num the camera to get, can be the player number (PLAYER_1,2,3,4)
 * @return pointer to the camera at that position in the camera list
 */
Camera *get_camera_by_num(int camera_num);


#endif