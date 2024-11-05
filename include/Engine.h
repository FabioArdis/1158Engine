#pragma once
#include "Editor.h"
#include "Renderer.h"
#include "SceneManager.h"

/**
 * @class Engine
 * @brief Manages the game loop and overall engine functionality.
 * 
 * The Engine class is responsible for initializing and running the game,
 * managing the rendering process, and handling scene management. It
 * serves as the central hub for the game engine's operations.
 */
class Engine {
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

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine(Engine&&) = delete;
  Engine& operator=(Engine&&) = delete;

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
  /** Pointer to the Renderer responsible for rendering graphics. */
  std::shared_ptr<Renderer> m_renderer{nullptr};
  /**Pointer to the SceneManager that handles scene transitions and GameObject management. */
  std::shared_ptr<SceneManager> m_sceneManager{nullptr};
  std::shared_ptr<Editor> m_editor{nullptr};
  /**Flag indicating whether the engine is currently running. */
  bool m_isRunning{false};
};
