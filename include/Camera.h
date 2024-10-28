#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class Camera
 * @brief Handles camera positioning and movement in the 3D space.
 * 
 * The Camera class is responsible for managing the camera's position and orientation,
 * allowing for movement and rotation in a 3D environment. It uses the GLM library 
 * for handling vectors and matrices.
 */
class Camera
{
public:
	/**
	* @brief Constructs a Camera object with specified position and orientation.
	* 
	* @param position The initial position of the camera in 3D space.
	* @param yaw The initial yaw (rotation around the Y-axis) of the camera.
	* @param pitch The initial pitch (rotation around the X-axis) of the camera.
	*/
	Camera(glm::vec3 position, float yaw, float pitch);

	/**
	* @brief Generates the view matrix for the camera.
	* 
	* @return The view matrix representing the camera's orientation and position.
	*/
	glm::mat4 GetViewMatrix();

	/**
	* @brief Processes keyboard input to move the camera.
	* 
	* @param deltaTime Time elapsed since the last frame, used for movement speed adjustment.
	* @param forward Whether to move the camera forward.
	* @param backward Whether to move the camera backward.
	* @param left Whether to move the camera left.
	* @param right Whether to move the camera right.
	*/
	void ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);
	
	/**
	* @brief Processes mouse movement to rotate the camera.
	* 
	* @param xOffset The change in the mouse's X position.
	* @param yOffset The change in the mouse's Y position.
	* @param constrainPitch Whether to constrain the pitch to prevent flipping.
	*/
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	/**
	* @brief Moves the camera forward in the direction it is facing.
	*/
	void MoveForward();

	/**
	* @brief Moves the camera backward, opposite to the direction it is facing.
	*/
	void MoveBackward();

	/**
	* @brief Moves the camera to the left.
	*/
	void MoveLeft();

	/**
	* @brief Moves the camera to the right.
	*/
	void MoveRight();

	void MoveUp();

	void MoveDown();

	void RotateLeft();

	void RotateRight();

	/**
	* @brief Rotates the camera based on mouse movement.
	* 
	* @param deltaX The change in the X-axis for rotation.
	* @param deltaY The change in the Y-axis for rotation.
	*/
	void Rotate(float deltaX, float deltaY);

	/**
	* @brief Retrieves the current position of the camera.
	* 
	* @return The position of the camera as a glm::vec3.
	*/
	glm::vec3 GetPosition() const { return m_position; }

private:

	glm::vec3 m_position;         /**The current position of the camera. */
	glm::vec3 m_front;            /**The direction the camera is facing. */
	glm::vec3 m_up;               /**The up direction of the camera. */
	glm::vec3 m_right;            /**The right direction of the camera. */
	glm::vec3 m_worldUp;          /**The world up direction. */

	float m_speed = 0.5f;         /**Movement speed of the camera. */
	float m_yaw;                  /**The yaw angle of the camera. */
	float m_pitch;                /**The pitch angle of the camera. */
	float m_movementSpeed;        /**The speed at which the camera moves. */
	float m_mouseSensitivity;     /**The sensitivity of mouse movement for rotation. */

	/**
	* @brief Updates the camera's direction vectors based on the current yaw and pitch.
	*/
	void UpdateCameraVectors();
};

