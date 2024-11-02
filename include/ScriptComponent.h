#pragma once

#include "Component.h"
#include <string>
#include <filesystem>

class ScriptBase;

class ScriptComponent : public Component{
private:
    std::string m_ScriptName;
    void* m_DllHandle = nullptr;
    ScriptBase* m_Instance = nullptr;
    std::filesystem::file_time_type m_LastCompileTime;

    void CompileAndLoad();
    void UnloadScript();
    void CompileScript();

    std::string GetScriptPath() const;
    std::string GetCompiledPath() const;
public:
    ScriptComponent(GameObject* owner) : Component(owner)
    {}
    ~ScriptComponent();

    std::string GetName() const { return m_ScriptName; }

    void LoadScript(const std::string& scriptName);
    void ReloadIfNeeded();
    ScriptBase* GetScriptInstance() { return m_Instance; }

    void Update(float deltaTime) override;

};
