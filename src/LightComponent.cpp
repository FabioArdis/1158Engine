#include "LightComponent.h"

#include "GameObject.h"

LightComponent::LightComponent(std::shared_ptr<GameObject> owner)
    : Component(owner),
      m_color(1.0f, 1.0f, 1.0f),
      m_intensity(1.0f),
      m_range(10.0f) {}

void LightComponent::Update(float deltaTime) {
  // Sync light's position and direction with the owner's transform
  auto owner = m_owner.lock();
  if (owner) {
    auto* transform = owner->GetComponent<TransformComponent>();

    if (transform != nullptr) {
      m_position = transform->GetPosition();
      m_direction = transform->GetForward();
    }
  }
}
