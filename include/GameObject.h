#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Component.h"
#include "LightComponent.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"


/**
 * @class GameObject
 * @brief Represents a game object that can have various components attached.
 * 
 * The GameObject class serves as a container for different components,
 * allowing for modular functionality in the game engine. It manages
 * components, updates their state, and handles rendering.
 */
class GameObject : public std::enable_shared_from_this<GameObject> {
 public:
  /**
     * @brief Constructs a GameObject with given name.
     * 
     * Initializes a TransformComponent and adds it to the game object.
     * Every GameObject must have at least a TransformComponent.
	 * 
     * @param name The name of the game object.
     */
  explicit GameObject(std::string name)
      : m_name(std::move(name)), m_id(nextID++) {
    std::cout << "Creating GameObject: " << m_name << std::endl;
  }

  void Initialize() {
    //std::cout << "Running transformComponent = std::make_unique<TransformComponent>(shared_from_this()); from GameObject::Initialize()\n";
    if (!transformComponent) {
      transformComponent =
          std::make_unique<TransformComponent>(shared_from_this());
      m_components.push_back(std::move(transformComponent));
    }
  }

  /**
     * @brief Destroys the GameObject and cleans up components.
     * 
     * Deletes all components associated with the game object to free resources.
     */
  ~GameObject() {
    std::cout << "Destroying GameObject: " << m_name << std::endl;
  }

  /**
     * @brief Adds a component to the game object.
     * 
     * @param component Pointer to the component to be added.
     * 
     * This method prevents adding multiple TransformComponents.
     */
  void AddComponent(std::unique_ptr<Component> component) {
    if (dynamic_cast<TransformComponent*>(component.get()) != nullptr) {
      return;
    }
    m_components.push_back(std::move(component));
  }

  /**
     * @brief Template method to add a component of a specific type.
     * 
     * @tparam T The type of the component to add.
     * @tparam Args Variadic template parameters for constructor arguments.
     * 
     * @param args Constructor arguments for the component.
     * @return Pointer to the newly created component, or nullptr if the component is TransformComponent.
     */
  template <typename T, typename... Args>
  std::unique_ptr<T> AddComponent(Args&&... args) {
    if (std::is_same<T, TransformComponent>::value) {
      return nullptr;
    }

    auto component =
        std::make_unique<T>(shared_from_this(), std::forward<Args>(args)...);

    m_components.push_back(std::move(component));

    return component;
  }

  /**
     * @brief Retrieves a component of a specified type.
     * 
     * @tparam T The type of the component to retrieve.
     * @return Pointer to the component of type T, or nullptr if not found.
     */
  template <typename T>
  T* GetComponent() {
    for (const auto& component : m_components) {
      if (auto castedComponent = dynamic_cast<T*>(component.get())) {
        return castedComponent;
      }
    }

    return nullptr;
  }

  /**
     * @brief Updates the game object and its components.
     * 
     * @param deltaTime The time elapsed since the last update.
     */
  void Update(float deltaTime) {
    //std::cout << "\tUpdating " << m_name << "." << std::endl; Will reuse this in the Log Dock
    for (const auto& component : m_components) {
      component->Update(deltaTime);
    }
  }

  /**
     * @brief Renders the game object and its components.
     */
  void Render() {
    //std::cout << "\tRendering " << m_name << ".\n";
    for (const auto& component : m_components) {
      component->Render();
    }
  }

  /**
     * @brief Retrieves the name of the game object.
     * 
     * @return The name of the game object.
     */
  [[nodiscard]] std::string GetName() const { return m_name; }

  /**
     * @brief Sets the name of the game object.
     * 
     * @param name The new name for the game object.
     */
  void SetName(const std::string& name) { m_name = name; }

  [[nodiscard]] unsigned int GetID() const { return m_id; }

 private:
  std::string m_name; /**The name of the game object. */
  std::vector<std::unique_ptr<Component>> m_components;
  std::unique_ptr<TransformComponent> transformComponent{nullptr};
  unsigned int m_id;
  static unsigned int nextID;
};