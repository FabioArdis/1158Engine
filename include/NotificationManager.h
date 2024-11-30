#pragma once

#include "core/Notification.h"
#include <deque>

class NotificationManager {
 public:
  NotificationManager() = default;
  void AddNotification(const std::string& message, float duration = 3.0f);
  void RenderNotifications();

 private:
  std::deque<Notification> m_notifications;
};
