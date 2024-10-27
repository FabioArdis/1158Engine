#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

/**
 * @class InputManager
 * @brief Manages input from the keyboard and mouse.
 * 
 * The InputManager class handles polling for input events and provides methods
 * to check key states and retrieve mouse position and movement. It interacts 
 * with GLFW to process input.
 */
class InputManager
{
public:
	/**
	* @brief Constructs an InputManager for the specified GLFW window.
	* 
	* @param window Pointer to the GLFW window to capture input from.
	*/
	InputManager(GLFWwindow* window);

	/**
	* @brief Polls for input events and updates the key states.
	* 
	* This method should be called each frame to process input events.
	*/
	void PollEvents();

	/**
	* @brief Checks if a specified key is currently pressed.
	* 
	* @param key The GLFW key code for the key to check.
	* @return True if the key is pressed, false otherwise.
	*/
	bool IsKeyPressed(int key);

	/**
	* @brief Retrieves the current mouse position.
	* 
	* @param x Reference to a double to store the mouse's X position.
	* @param y Reference to a double to store the mouse's Y position.
	*/
	void GetMousePosition(double& x, double& y);

	/**
	* @brief Retrieves the mouse movement since the last frame.
	* 
	* @param deltaX Reference to a double to store the change in mouse X position.
	* @param deltaY Reference to a double to store the change in mouse Y position.
	*/
	void GetMouseDelta(double& deltaX, double& deltaY);

private:
    GLFWwindow* m_window;                    	/**Pointer to the GLFW window for input handling. */
    std::unordered_map<int, bool> m_keyState;	/**Map to store the current state of keys. */

    double m_prevMouseX;                     	/**Previous mouse X position for delta calculation. */
    double m_prevMouseY;                     	/**Previous mouse Y position for delta calculation. */
};