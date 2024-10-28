#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "InputManager.h"
#include "Light.h"
#include "Scene.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "TextRenderer.h"
#include "Editor.h"

/**
 * @class Renderer
 * @brief Handles rendering of scenes and objects using OpenGL.
 * 
 * The Renderer class is responsible for initializing OpenGL, creating a 
 * window with GLFW, managing shaders, and rendering objects in a scene.
 */
class Renderer
{
public:
	/**
	* @brief Constructs a Renderer instance.
	*/
	Renderer();

	/**
	* @brief Destructs the Renderer instance.
	*/
	~Renderer();

	/**
	* @brief Initializes the Renderer and GLFW.
	* 
	* @return True if initialization succeeded, false otherwise.
	*/
	bool Initialize();

	/**
	* @brief Cleans up resources and shuts down the Renderer.
	*/
	void Shutdown();

	/**
	* @brief Checks if the rendering window should close.
	* 
	* @return True if the window should close, false otherwise.
	*/
	bool ShouldClose();

	/**
	* @brief Renders the specified scene.
	* 
	* @param scene The scene to render.
	*/
	void Render(Scene* scene);

	/**
	* @brief Sets the camera for the Renderer.
	* 
	* @param camera The camera to be set.
	*/
	void SetCamera(Camera* camera);

	/**
	* @brief Sets the lights for the scene.
	* 
	* @param lights A vector of lights to be set in the scene.
	*/
	void SetLights(const std::vector<Light>& lights);

	/**
	* @brief Calculates and updates the frames per second (FPS).
	*/
	void CalculateFPS();

	void SetEditor(Editor* editor) { m_editor = editor; }

	GLFWwindow* GetWindow() const { return m_window; }

private:
    GLFWwindow* m_window; 					/**Pointer to the GLFW window. */
    Shader* m_shader; 						/**Pointer to the active shader. */
    Mesh* m_mesh; 							/**Pointer to the mesh (currently unused, to be removed). */
    Camera* m_camera; 						/**Pointer to the camera used for rendering. */
    InputManager* m_inputManager; 			/**Pointer to the input manager. */
    ShaderManager* m_shaderManager; 		/**Pointer to the shader manager. */
    TextRenderer* m_textRenderer; 			/**Pointer to the text renderer. */
	Editor* m_editor;

    std::vector<Light> m_lights; 			/**Vector of lights in the scene. */
	
	/**
	* @brief Renders a single GameObject.
	* 
	* @param object The GameObject to render.
	*/
	void RenderObject(GameObject* object);

    double m_lastTime;	/**Last recorded time for FPS calculations. */
    int m_nbFrames; 	/**Number of frames rendered in the last second. */
    double m_fps; 		/**Current frames per second. */
    double m_frameTime; /**Time taken to render the last frame. */
};

