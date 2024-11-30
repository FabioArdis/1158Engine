#pragma once

#include <string>
#include <chrono>


class Notification {
 public:
  Notification(const std::string& message, float duration)
     : m_message(message), m_duration(duration),
       m_startTime(std::chrono::steady_clock::now()) {}

  const std::string& GetMessage() const { return m_message; }

  bool IsExpired() const {
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - m_startTime).count();
    return elapsed >= m_duration;
  }
 private:
  std::string m_message;
  float m_duration;
  std::chrono::time_point<std::chrono::steady_clock> m_startTime;
};



