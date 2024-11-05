#pragma once

#include "GameObject.h"
#include "core/PropertyTypes.h"

class ScriptBase {
 public:
  virtual ~ScriptBase() = default;

  virtual void OnCreate() {}

  virtual void OnUpdate(float deltaTime) {}

  virtual void OnDestroy() {}

  template <typename T>
  T* GetComponent() {
    if (!m_gameObject) {
      std::cerr << "GameObject is not set." << std::endl;
      return nullptr;
    }
    return m_gameObject->template GetComponent<T>();
  }

  [[nodiscard]] virtual std::vector<std::pair<std::string, PropertyType>>
  GetProperties() const = 0;

  virtual void* GetPropertyPtr(const std::string& name) = 0;

 private:
  std::shared_ptr<GameObject> m_gameObject = nullptr;
  friend class ScriptComponent;
};

#ifdef _WIN32
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT
#endif