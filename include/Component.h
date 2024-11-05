#pragma once

#include <memory>

class GameObject;

/**
 * @class Component
 * @brief Base class for components attached to GameObjects.
 *
 * The Component class serves as a base for all components that can be attached
 * to game objects in the game engine. It provides an interface for updating and
 * rendering components.
 */
class Component {
 public:
  /**
  * @brief Constructs a Component with the specified owner GameObject.
  *
  * @param owner Pointer to the GameObject that owns this component.
  */
  explicit Component(std::weak_ptr<GameObject> owner)
      : m_owner(std::move(owner)) {};

  // We probably won't need them right now until we'll implement duplicating.
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;
  Component(Component&&) = delete;
  Component& operator=(Component&&) = delete;

  /**
  * @brief Virtual destructor for proper cleanup of derived classes.
  */
  virtual ~Component() = default;

  /**
  * @brief Updates the component state.
  *
  * This method is called every frame to update the component.
  *
  * @param deltaTime The time elapsed since the last frame.
  */
  virtual void Update(float deltaTime) = 0;

  /**
  * @brief Renders the component.
  *
  * This method is called to render the component's visual representation.
  * The default implementation does nothing; derived classes can override it.
  */
  virtual void Render() {};

 protected:
  /** Weak pointer to the GameObject that owns this component. */
  std::weak_ptr<GameObject> m_owner;
};