#include "LightComponent.h"
#include "GameObject.h"

LightComponent::LightComponent(GameObject* owner) : Component(owner), m_color(1.0f, 1.0f, 1.0f), m_intensity(1.0f), m_range(10.0f) {}

void LightComponent::Update(float deltaTime)
{
	// Sync light's position and direction with the owner's transform
	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();

	if (transform)
	{
		m_position = transform->GetPosition();
		m_direction = transform->GetForward();
	}
}
