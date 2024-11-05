#include "Camera.h"

// Initialize camera with position, yaw, and pitch
Camera::Camera(glm::vec3 position, float yaw, float pitch)
    : m_position(position),
      m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      m_yaw(yaw),
      m_pitch(pitch),
      m_movementSpeed(2.5f),
      m_mouseSensitivity(0.1f) {
  UpdateCameraVectors();
}

// Return the view matrix based on position and direction
glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(m_position, m_position + m_front, m_up);
}

// Move camera based on keyboard input
void Camera::ProcessKeyboard(float deltaTime, bool forward, bool backward,
                             bool left, bool right) {
  float velocity = m_movementSpeed * deltaTime;
  if (forward)
    m_position += m_front * velocity;
  if (backward)
    m_position -= m_front * velocity;
  if (left)
    m_position -= m_right * velocity;
  if (right)
    m_position += m_right * velocity;
}

// Adjust yaw and pitch based on mouse movement
void Camera::ProcessMouseMovement(float xOffset, float yOffset,
                                  bool constrainPitch) {
  xOffset += m_mouseSensitivity;
  yOffset += m_mouseSensitivity;

  m_yaw += xOffset;
  m_pitch += yOffset;

  // Constrain pitch to avoid flipping
  if (constrainPitch) {
    if (m_pitch > 89.0f)
      m_pitch = 89.0f;
    if (m_pitch < -89.0f)
      m_pitch = -89.0f;
  }

  UpdateCameraVectors();
}

// Move camera forward
void Camera::MoveForward() {
  m_position += m_front * m_speed;
}

// Move camera backward
void Camera::MoveBackward() {
  m_position -= m_front * m_speed;
}

// Move camera left
void Camera::MoveLeft() {
  m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
}

// Move camera right
void Camera::MoveRight() {
  m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;
}

void Camera::RotateLeft() {
  m_yaw -= 0.5f * 10.0f;
  UpdateCameraVectors();
}

void Camera::RotateRight() {
  m_yaw += 0.5f * 10.0f;
  UpdateCameraVectors();
}

void Camera::MoveUp() {
  m_position.y += m_speed;
}

void Camera::MoveDown() {
  m_position.y -= m_speed;
}

// Rotate camera based on input deltas
void Camera::Rotate(float deltaX, float deltaY) {
  m_yaw += deltaX * m_mouseSensitivity;
  m_pitch -= deltaY * m_mouseSensitivity;

  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;

  UpdateCameraVectors();
}

// Recalculate front, right, and up vectors based on updated yaw and pitch
void Camera::UpdateCameraVectors() {
  glm::vec3 front;

  front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  front.y = glm::sin(glm::radians(m_pitch));
  front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up = glm::normalize(glm::cross(m_right, m_front));
  m_front = glm::normalize(front);
}