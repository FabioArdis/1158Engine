#include "NotificationManager.h"
#include "imgui.h"

void NotificationManager::AddNotification(const std::string& message,
                                          float duration) {
  m_notifications.emplace_back(message, duration);
}

void NotificationManager::RenderNotifications() {
  float yOffset = 100.0f;
  const float maxWidth = 250.0f; // Larghezza massima per la finestra di notifica

  auto it = m_notifications.begin();
  while (it != m_notifications.end()) {
    if (it->IsExpired()) {
      it = m_notifications.erase(it);
    } else {
      // Imposta la posizione e le dimensioni della notifica
      ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - maxWidth - 20, ImGui::GetIO().DisplaySize.y - yOffset), ImGuiCond_Always);
      ImGui::SetNextWindowSize(ImVec2(maxWidth, 0), ImGuiCond_Always);

      ImGui::Begin("##notification", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxWidth);
      ImGui::TextWrapped("%s", it->GetMessage().c_str());
      ImGui::PopTextWrapPos();
      ImGui::End();

      // Aggiorna l'offset in base all'altezza della notifica corrente
      yOffset += ImGui::GetWindowHeight() + 5.0f; // Aggiungi uno spazio tra le notifiche
      ++it;
    }
  }
}