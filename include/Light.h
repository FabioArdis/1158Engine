#pragma once
#include <glm/glm.hpp>

/**
 * @class Light
 * @brief Represents a light source in the scene.
 * 
 * The Light class encapsulates the properties of a light source, including
 * its position, color, and intensity. It provides methods for accessing and 
 * modifying these properties.
 */
class Light {
 public:
  /**
	* @brief Constructs a Light object with specified properties.
	* 
	* @param position The position of the light in world coordinates.
	* @param color The color of the light (RGB).
	* @param intensity The intensity of the light source.
	*/
  Light(const glm::vec3& position, const glm::vec3& color, float intensity);

  /**
	* @brief Retrieves the position of the light.
	* 
	* @return The position of the light as a glm::vec3.
	*/
  [[nodiscard]] glm::vec3 GetPosition() const;

  /**
	* @brief Retrieves the color of the light.
	* 
	* @return The color of the light as a glm::vec3.
	*/
  [[nodiscard]] glm::vec3 GetColor() const;

  /**
	* @brief Retrieves the intensity of the light.
	* 
	* @return The intensity of the light as a float.
	*/
  [[nodiscard]] float GetIntensity() const;

  /**
	* @brief Sets the position of the light.
	* 
	* @param position The new position of the light as a glm::vec3.
	*/
  void SetPosition(const glm::vec3& position);

  /**
	* @brief Sets the color of the light.
	* 
	* @param color The new color of the light as a glm::vec3.
	*/
  void SetColor(const glm::vec3& color);

  /**
	* @brief Sets the intensity of the light.
	* 
	* @param intensity The new intensity of the light as a float.
	*/
  void SetIntensity(const float intensity);

 private:
  glm::vec3 m_position; /**The position of the light in world space. */
  glm::vec3 m_color;    /**The color of the light (RGB). */
  float m_intensity;    /**The intensity of the light source. */
};
