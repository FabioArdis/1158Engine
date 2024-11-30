#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Component.h"

/**
 * @class LightComponent
 * @brief Represents a light source component attached to a GameObject.
 * 
 * The LightComponent class extends the Component class and encapsulates
 * the properties of a light source, including color, intensity, and range.
 * It also provides methods for updating the component's state within the 
 * game loop.
 */
class LightComponent : public Component {
 public:
  /**
     * @brief Constructs a LightComponent for the specified GameObject.
     * 
     * @param owner Pointer to the GameObject that owns this light component.
     */
  LightComponent(const std::shared_ptr<GameObject>& owner);

  /**
     * @brief Updates the light component's state, including its position and direction based on owner's transform.
     * @param deltaTime The time elapsed since the last update.
     */
  void Update(float deltaTime) override;

  // Setters

  /**
     * @brief Sets the color of the light.
     * 
     * @param color The new color of the light as a glm::vec3.
     */
  void SetColor(const glm::vec3& color) { m_color = color; }

  /**
     * @brief Sets the intensity of the light.
     * 
     * @param intensity The new intensity of the light as a float.
     */
  void SetIntensity(float intensity) { m_intensity = intensity; }

  /**
     * @brief Sets the range of the light.
     * 
     * @param range The new range of the light as a float.
     */
  void SetRange(float range) { m_range = range; }

  // Getters

  /**
     * @brief Retrieves the color of the light.
     * 
     * @return The color of the light as a glm::vec3.
     */
  [[nodiscard]] const glm::vec3& GetColor() const { return m_color; }

  /**
     * @brief Retrieves the intensity of the light.
     * 
     * @return The intensity of the light as a float.
     */
  [[nodiscard]] float GetIntensity() const { return m_intensity; }

  /**
     * @brief Retrieves the range of the light.
     * 
     * @return The range of the light as a float.
     */
  [[nodiscard]] float GetRange() const { return m_range; }

  /**
     * @brief Retrieves the position of the light.
     * 
     * @return The position of the light as a glm::vec3.
     */
  [[nodiscard]] const glm::vec3& GetPosition() const { return m_position; }

  /**
     * @brief Retrieves the direction of the light.
     * 
     * @return The direction of the light as a glm::vec3.
     */
  [[nodiscard]] const glm::vec3& GetDirection() const { return m_direction; }

 private:
  glm::vec3 m_color;    /**The color of the light (RGB). */
  float m_intensity;    /**The intensity of the light source. */
  float m_range;        /**The effective range of the light source. */
  glm::vec3 m_position; /**The position of the light in world space. */
  /**The direction of the light (for directional lights). */
  glm::vec3 m_direction;
};
