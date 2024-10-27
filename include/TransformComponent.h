#pragma once
#include "Component.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class TransformComponent
 * @brief Manages the position, rotation, and scale of a GameObject in 3D space.
 * 
 * The TransformComponent class provides functionalities for setting and retrieving
 * the transformation properties of a GameObject, including position, rotation, 
 * and scale. It also allows generating a transformation matrix for rendering.
 */
class TransformComponent : public Component
{
public:
	/**
	 * @brief Constructs a TransformComponent associated with the specified GameObject.
	 * 
	 * Initializes position to (0, 0, 0), rotation to (0, 0, 0), and scale to (1, 1, 1).
	 * @param owner Pointer to the GameObject that owns this component.
	 */
	TransformComponent(GameObject* owner) : Component(owner), m_position(0.0f), m_rotation(0.0f), m_scale(1.0f) {}

	/**
	 * @brief Calculates the transformation matrix for the GameObject.
	 * 
	 * The matrix is based on the current position, rotation, and scale of the object.
	 * @return A 4x4 transformation matrix.
	 */
	glm::mat4 GetTransformMatrix() const {
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(transform, m_position);
		transform = glm::rotate(transform, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, m_scale);

		return transform;
	}

	/**
	 * @brief Calculates the forward vector based on the current rotation.
	 * 
	 * Uses the rotation to determine the forward direction in the local space.
	 * @return A glm::vec3 representing the forward direction.
	 */
	glm::vec3 GetForward() const
	{
		// Calcoliamo la matrice di rotazione basata sull'orientamento dell'oggetto
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

		// Il vettore forward di default � (0, 0, -1)
		glm::vec4 forward = rotationMatrix * glm::vec4(0, 0, -1, 0);

		// Ritorniamo il forward come glm::vec3
		return glm::vec3(forward);
	}

	/**
	 * @brief Updates the component's state each frame.
	 * 
	 * @param deltaTime Time elapsed since the last frame (in seconds).
	 */
	void Update(float deltaTime) override {}

	/**
	 * @brief Sets the position of the GameObject.
	 * 
	 * @param position New position in 3D space.
	 */
	void SetPosition(glm::vec3 position)
	{
		/*std::cout << "Setting position from (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ") to (" 
			<< position.x << ", " << position.y << ", " << position.z << ")" << std::endl;*/
		m_position = position;
	}

	/**
	 * @brief Sets the rotation of the GameObject.
	 * 
	 * @param rotation New rotation values for the x, y, and z axes (in degrees).
	 */
	void SetRotation(glm::vec3 rotation)
	{

		m_rotation = rotation;
	}

	/**
	 * @brief Sets the scale of the GameObject.
	 * 
	 * @param scale New scale values for the x, y, and z axes.
	 */
	void SetScale(glm::vec3 scale)
	{

		m_scale = scale;
	}

	/**
	 * @brief Retrieves the current position of the GameObject.
	 * 
	 * @return The position as a glm::vec3.
	 */
	glm::vec3 GetPosition() const
	{
		return m_position;
	}

private:
	glm::vec3 m_position; /**Position of the GameObject in 3D space. */
	glm::vec3 m_rotation; /**Rotation of the GameObject in degrees (x, y, z axes). */
	glm::vec3 m_scale;    /**Scale of the GameObject along the x, y, and z axes. */
};

