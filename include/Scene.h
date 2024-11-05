#pragma once
#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>
#include "GameObject.h"

/**
 * @class Scene
 * @brief Represents a collection of GameObjects to be managed and rendered.
 * 
 * The Scene class handles the storage, updating, and rendering of GameObjects 
 * within a specific scene. It allows for adding, removing, and iterating over 
 * the game objects to facilitate game logic and rendering.
 */
class Scene {
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
  Scene(const std::string& sceneName) : m_name(sceneName) {}

  /**
	 * @brief Destructs the Scene and cleans up resources.
	 * 
	 * Deletes all GameObjects contained in the scene.
	 */
  ~Scene() = default;

  bool IsNameTaken(const std::string& name) const {
    return m_objectsNames.find(name) != m_objectsNames.end();
  }

  std::string GenerateUniqueName(const std::string& name) const {
    std::string uniqueName = name;
    int counter = 1;

    while (IsNameTaken(uniqueName)) {
      uniqueName = name + std::to_string(counter++);
    }

    return uniqueName;
  }

  /**
	 * @brief Gets the list of GameObjects in the scene.
	 * 
	 * @return A vector of pointers to GameObjects in the scene.
	 */
  std::vector<std::shared_ptr<GameObject>> GetGameObjects() {
    return m_gameObjects;
  }

  /**
	 * @brief Adds a GameObject to the scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be added.
	 */
  void AddGameObject(std::shared_ptr<GameObject> gameObject) {
    m_gameObjects.push_back(gameObject);
    m_objectsNames.insert(gameObject->GetName());
  }

  /**
	 * @brief Removes a GameObject from the scene.
	 * 
	 * @param gameObject Pointer to the GameObject to be removed.
	 */
  void RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
    auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);

    if (it != m_gameObjects.end()) {
      m_objectsNames.erase((*it)->GetName());
      m_gameObjects.erase(it);
    }
  }

  /**
	 * @brief Updates all GameObjects in the scene.
	 * 
	 * Calls the Update method of each GameObject, passing the delta time.
	 * 
	 * @param deltaTime The time elapsed since the last update.
	 */
  void Update(float deltaTime) {
    //std::cout << "Updating " << m_name << "." << std::endl; Will reuse in the Log Dock
    for (const auto& gameObject : m_gameObjects) {
      gameObject->Update(deltaTime);
    }
  }

  /**
	 * @brief Renders all GameObjects in the scene.
	 * 
	 * Calls the Render method of each GameObject to draw them.
	 */
  void Render() {
    for (const auto& gameObject : m_gameObjects) {
      gameObject->Render();
    }
  }

 private:
  /**The name of the scene. */
  std::string m_name;
  /**Vector containing pointers to GameObjects in the scene. */
  std::vector<std::shared_ptr<GameObject>> m_gameObjects;
  std::unordered_set<std::string> m_objectsNames;
  // Since i'm just doing name checking, i have chosen to go with hashing. Much faster with a little memory overhead.
};
