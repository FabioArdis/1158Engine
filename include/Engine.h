#pragma once
#include "Renderer.h"
#include "SceneManager.h"
#include "Editor.h"

/**
 * @class Engine
 * @brief Manages the game loop and overall engine functionality.
 * 
 * The Engine class is responsible for initializing and running the game,
 * managing the rendering process, and handling scene management. It
 * serves as the central hub for the game engine's operations.
 */
class Engine
{
public:
	/**
	* @brief Constructs the Engine object.
	*/
	Engine();

	/**
	* @brief Destroys the Engine object.
	* 
	* Cleans up resources and performs necessary shutdown procedures.
	*/
	~Engine();

	/**
	* @brief Initializes the engine components.
	* 
	* This method sets up the renderer, scene manager, and other necessary
	* components for the engine to run.
	* 
	* @return True if initialization was successful, false otherwise.
	*/
	bool Initialize();

	/**
	* @brief Runs the main game loop.
	* 
	* This method handles the game loop, including updating game state,
	* processing input, and rendering frames.
	*/
	void Run();

	/**
	* @brief Shuts down the engine and releases resources.
	* 
	* This method cleans up any resources allocated during the engine's
	* runtime and prepares the engine for termination.
	*/
	void Shutdown();

private:
	Renderer* m_renderer;    /**Pointer to the Renderer responsible for rendering graphics. */
	SceneManager* m_sceneManager; /**Pointer to the SceneManager that handles scene transitions and GameObject management. */
	Editor* m_editor;
	bool m_isRunning;        /**Flag indicating whether the engine is currently running. */
};

