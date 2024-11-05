#pragma once
#include <vector>
#include "Scene.h"

/**
 * @class SceneManager
 * @brief Manages the scenes in the game.
 * 
 * The SceneManager class is responsible for creating, destroying, 
 * and managing the Scenes in the game. It provides methods to add and remove 
 * GameObjects to the current scene and to update and render the scene.
 */
class SceneManager {
 public:
  /**
	 * @brief Constructs a new SceneManager and initializes the current scene to nullptr.
	 */
  SceneManager() {}

  /**
	 * @brief Destructs the SceneManager and cleans up resources.
	 * 
	 * Calls DestroyScene to delete the current scene if it exists.
	 */
  ~SceneManager() { DestroyScene(); }

  /**
	 * @brief Creates a new scene and sets it as the current scene.
	 * 
	 * Allocates memory for a new Scene object.
	 */
  void CreateScene() { currentScene = std::make_shared<Scene>(); }

  /**
	 * @brief Destroys the current scene.
	 * 
	 * Deletes the current Scene object and sets the pointer to nullptr.
	 */
  void DestroyScene() {
    //delete currentScene;
    currentScene = nullptr;
  }

  /**
	 * @brief Adds a GameObject to the current scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be added.
	 * 
	 * If there is no current scene, the GameObject will not be added.
	 */
  void AddObjectToCurrentScene(std::shared_ptr<GameObject> gameObject) {
    if (currentScene != nullptr) {
      currentScene->AddGameObject(gameObject);
    }
  }

  /**
	 * @brief Removes a GameObject from the current scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be removed.
	 * 
	 * If there is no current scene, the GameObject will not be removed.
	 */
  void RemoveObjectFromCurrentScene(std::shared_ptr<GameObject> gameObject) {
    if (currentScene != nullptr) {
      currentScene->RemoveGameObject(gameObject);
    }
  }

  /**
	 * @brief Updates the current scene.
	 * 
	 * Calls the Update method of the current scene, passing the delta time.
	 * 
	 * @param deltaTime The time elapsed since the last update.
	 */
  void UpdateCurrentScene(float deltaTime) {
    if (currentScene != nullptr) {
      currentScene->Update(deltaTime);
    }
  }

  /**
	 * @brief Renders the current scene.
	 * 
	 * Calls the Render method of the current scene.
	 */
  void RenderCurrentScene() {
    if (currentScene != nullptr) {
      currentScene->Render();
    }
  }

  /**
	 * @brief Gets the current scene.
	 * 
	 * @return A pointer to the current Scene object, or nullptr if none exists.
	 */
  std::shared_ptr<Scene> GetScene() { return currentScene; }

 private:
  /**Pointer to the current scene being managed. */
  std::shared_ptr<Scene> currentScene{nullptr};
};
