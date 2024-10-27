#pragma once
#include "Scene.h"
#include <vector>

/**
 * @class SceneManager
 * @brief Manages the scenes in the game.
 * 
 * The SceneManager class is responsible for creating, destroying, 
 * and managing the Scenes in the game. It provides methods to add and remove 
 * GameObjects to the current scene and to update and render the scene.
 */
class SceneManager
{
public:
	/**
	 * @brief Constructs a new SceneManager and initializes the current scene to nullptr.
	 */
	SceneManager() : currentScene(nullptr) {}

	/**
	 * @brief Destructs the SceneManager and cleans up resources.
	 * 
	 * Calls DestroyScene to delete the current scene if it exists.
	 */
	~SceneManager()
	{
		DestroyScene();
	}

	/**
	 * @brief Creates a new scene and sets it as the current scene.
	 * 
	 * Allocates memory for a new Scene object.
	 */
	void CreateScene()
	{
		currentScene = new Scene();
	}

	/**
	 * @brief Destroys the current scene.
	 * 
	 * Deletes the current Scene object and sets the pointer to nullptr.
	 */
	void DestroyScene()
	{
		delete currentScene;
		currentScene = nullptr;
	}
	
	/**
	 * @brief Adds a GameObject to the current scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be added.
	 * 
	 * If there is no current scene, the GameObject will not be added.
	 */
	void AddObjectToCurrentScene(GameObject* gameObject) {
		if (currentScene) {
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
	void RemoveObjectFromCurrentScene(GameObject* gameObject) {
		if (currentScene) {
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
		if (currentScene) {
			currentScene->Update(deltaTime);
		}
	}

	/**
	 * @brief Renders the current scene.
	 * 
	 * Calls the Render method of the current scene.
	 */
	void RenderCurrentScene() {
		if (currentScene) {
			currentScene->Render();
		}
	}

	/**
	 * @brief Gets the current scene.
	 * 
	 * @return A pointer to the current Scene object, or nullptr if none exists.
	 */
	Scene* GetScene()
	{
		return currentScene;
	}

private:
	Scene* currentScene;    /**Pointer to the current scene being managed. */
};

