#include "InputManager.h"
#include <iostream>

// Initialize with the GLFW window reference
InputManager::InputManager(GLFWwindow* window) : m_window(window), m_prevMouseX(0), m_prevMouseY(0) {}

void InputManager::PollEvents()
{
	glfwPollEvents();

	// Update key states for all possible keys
	for (int key = 0; key <= 1024; ++key)
	{
		m_keyState[key] = (glfwGetKey(m_window, key) == GLFW_PRESS);
	}

	// Toggle fullscreen/borderless on F11
	if (IsKeyPressed(GLFW_KEY_F11)) {
		static bool isBorderless = true;

		if (isBorderless) {
            glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowMonitor(m_window, nullptr, 100, 100, 1280, 720, 0);
        } else {
            glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_FALSE);
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }

		isBorderless = !isBorderless;
	}

	// Enable normal cursor mode on ESC
	if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Ritorna alla modalit� normale
	}
}

// Check if a key is currently pressed
bool InputManager::IsKeyPressed(int key)
{
	return m_keyState[key];
}

// Get current mouse position
void InputManager::GetMousePosition(double& x, double& y)
{
	glfwGetCursorPos(m_window, &x, &y);
}

// Calculate mouse movement delta since last frame
void InputManager::GetMouseDelta(double& deltaX, double& deltaY)
{
	double currentMouseX, currentMouseY;
	glfwGetCursorPos(m_window, &currentMouseX, &currentMouseY);

	deltaX = currentMouseX - m_prevMouseX;
	deltaY = currentMouseY - m_prevMouseY;

	m_prevMouseX = currentMouseX;
	m_prevMouseY = currentMouseY;
}
