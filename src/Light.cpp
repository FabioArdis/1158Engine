#include "Light.h"

// Initialize light with position, color, and intensity
Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity)
    : m_position(position), m_color(color), m_intensity(intensity) {}

glm::vec3 Light::GetPosition() const {
  return m_position;
}

glm::vec3 Light::GetColor() const {
  return m_color;
}

float Light::GetIntensity() const {
  return m_intensity;
}

void Light::SetPosition(const glm::vec3& position) {
  m_position = position;
}

void Light::SetColor(const glm::vec3& color) {
  m_color = color;
}

void Light::SetIntensity(const float intensity) {
  m_intensity = intensity;
}
