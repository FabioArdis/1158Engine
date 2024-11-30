#pragma once

#include <filesystem>
#include <string>
#include "Component.h"

class ScriptBase;

class ScriptComponent : public Component {
 private:
  bool compiled = false;
  std::string m_ScriptName;
  void* m_DllHandle = nullptr;
  ScriptBase* m_Instance = nullptr;
  std::filesystem::file_time_type m_LastCompileTime;

  std::string CompileAndLoad();
  void UnloadScript();
  void CompileScript();

  [[nodiscard]] std::string GetScriptPath() const;
  [[nodiscard]] std::string GetCompiledPath() const;

 public:
  explicit ScriptComponent(const std::shared_ptr<GameObject>& owner)
      : Component(owner) {}

  ~ScriptComponent() override;

  [[nodiscard]] std::string GetName() const { return m_ScriptName; }

  void LoadScript(const std::string& scriptName);
  std::string ReloadIfNeeded();

  ScriptBase* GetScriptInstance() const { return m_Instance; }

  void Update(float deltaTime) override;
};
