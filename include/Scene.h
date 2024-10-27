#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "GameObject.h"

/**
 * @class Scene
 * @brief Represents a collection of GameObjects to be managed and rendered.
 * 
 * The Scene class handles the storage, updating, and rendering of GameObjects 
 * within a specific scene. It allows for adding, removing, and iterating over 
 * the game objects to facilitate game logic and rendering.
 */
class Scene
{
public:
	/**
	 * @brief Constructs a new Scene with a default name.
	 */
	Scene() : m_name("NewScene") {}

	/**
	 * @brief Constructs a new Scene with a specified name.
	 * 
	 * @param sceneName The name of the scene.
	 */
	Scene(std::string sceneName) : m_name(sceneName) {} 

	/**
	 * @brief Destructs the Scene and cleans up resources.
	 * 
	 * Deletes all GameObjects contained in the scene.
	 */
	~Scene()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			delete gameObject;
		}
	}

	/**
	 * @brief Gets the list of GameObjects in the scene.
	 * 
	 * @return A vector of pointers to GameObjects in the scene.
	 */
	std::vector<GameObject*> GetGameObjects()
	{
		return m_gameObjects;
	}

	/**
	 * @brief Adds a GameObject to the scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be added.
	 */
	void AddGameObject(GameObject* gameObject)
	{
		m_gameObjects.push_back(gameObject);
	}

	/**
	 * @brief Removes a GameObject from the scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be removed.
	 */
	void RemoveGameObject(GameObject* gameObject)
	{
		m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(),gameObject), m_gameObjects.end());
	}

	/**
	 * @brief Updates all GameObjects in the scene.
	 * 
	 * Calls the Update method of each GameObject, passing the delta time.
	 * 
	 * @param deltaTime The time elapsed since the last update.
	 */
	void Update(float deltaTime)
	{
		std::cout << "Updating " << m_name << "." << std::endl;
		for (GameObject* gameObject : m_gameObjects)
		{
			gameObject->Update(deltaTime);
		}
	}

	/**
	 * @brief Renders all GameObjects in the scene.
	 * 
	 * Calls the Render method of each GameObject to draw them.
	 */
	void Render()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			gameObject->Render();
		}
	}

private:
	std::string m_name; 					/**The name of the scene. */
	std::vector<GameObject*> m_gameObjects; /**Vector containing pointers to GameObjects in the scene. */
};