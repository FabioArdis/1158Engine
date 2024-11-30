#include "ScriptComponent.h"

#include <stdexcept>

#include "ScriptBase.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

ScriptComponent::~ScriptComponent() {
  UnloadScript();
}

void ScriptComponent::LoadScript(const std::string& scriptName) {
  m_ScriptName = scriptName;
  CompileAndLoad();
}

std::string ScriptComponent::ReloadIfNeeded() {
  auto scriptPath = GetScriptPath();

  if (!std::filesystem::exists(scriptPath)) {
    // Handle error
    return "Error: " + scriptPath + " does not exist.";
  }

  auto currentTime = std::filesystem::last_write_time(scriptPath);

  if (currentTime > m_LastCompileTime) {
    CompileAndLoad();
  }

  return "";
}

std::string ScriptComponent::GetScriptPath() const {
  return "scripts/" + m_ScriptName + ".cpp";
}

std::string ScriptComponent::GetCompiledPath() const {
#ifdef _WIN32
  return "scripts/compiled/" + m_ScriptName + ".dll";
#else
  return "scripts/compiled/" + m_ScriptName + ".so";
#endif
}

void ScriptComponent::CompileScript() {
  std::filesystem::create_directories("scripts/compiled");

#ifdef _WIN32  // I... don't really want to think about this right now.
  std::string command = R"(g++ -fPIC -shared -std=c++17 -Iinclude -IC:\libs\GLM\ -O3 )" +
                        GetScriptPath() + " -o " + GetCompiledPath();
#else
  std::string command = "g++ -fPIC -shared -std=c++17 -Iinclude -O3 " +
                        GetScriptPath() + " -o " + GetCompiledPath();
#endif

  // std::cout << "pwd:";
  // system("pwd");

  // std::cerr << command << std::endl;

  int result = system(command.c_str());

  if (result != 0) {
    // Handle error
    // std::cerr << "ERROR DURING COMPILATION";
  } else {
    // std::cout << "COMPILED SUCCESSFULLY\n";
  }
}

void ScriptComponent::UnloadScript() {
  if (m_Instance != nullptr) {
    using DestroyScriptFn = void (*)(ScriptBase*);

#ifdef _WIN32
    auto destroyScript =
        reinterpret_cast<DestroyScriptFn>(
        GetProcAddress(static_cast<HMODULE>(m_DllHandle), "DestroyScript"));
#else
    auto destroyScript = (DestroyScriptFn)dlsym(m_DllHandle, "DestroyScript");
#endif

    if (destroyScript != nullptr) {
      m_Instance->OnDestroy();
      destroyScript(m_Instance);
    }

    m_Instance = nullptr;
  }

  if (m_DllHandle != nullptr) {
#ifdef _WIN32
    FreeLibrary((HMODULE)m_DllHandle);
#else
    dlclose(m_DllHandle);
#endif

    m_DllHandle = nullptr;
  }
}

std::string ScriptComponent::CompileAndLoad() {
  try {
    // std::cout << "start UnloadScript()\n";
    UnloadScript();

    // std::cout << "start CompileScript()\n";
    CompileScript();

    // std::cout << "start UnloadScript()\n";
    UnloadScript();

// std::cout << "start dlopen()\n";
#ifdef _WIN32
    m_DllHandle = LoadLibrary(GetCompiledPath().c_str());
#else
    m_DllHandle = dlopen(GetCompiledPath().c_str(), RTLD_NOW);
#endif

    // std::cout << "check dlopen(): GetCompiledPath:" + GetCompiledPath();
    if (m_DllHandle == nullptr) {
      compiled = false;
      // std::cerr << "dlopen error: " << dlerror() << std::endl;
      return "Failed to load script library: " + m_ScriptName + "(" + GetCompiledPath() + ")";
    }

#ifdef _WIN32
    auto createScript = reinterpret_cast<ScriptBase* (*)()>(
        GetProcAddress(static_cast<HMODULE>(m_DllHandle), "CreateScript"));
#else
    auto createScript = (ScriptBase * (*)()) dlsym(m_DllHandle, "CreateScript");
#endif

    if (createScript == nullptr) {
      throw std::runtime_error("Failed to get CreateScript function");
    }

    m_Instance = createScript();
    if (m_Instance == nullptr) {
      throw std::runtime_error("Failed to create script instance");
    }

    if (auto sharedOwner = m_owner.lock()) {
      m_Instance->m_gameObject = sharedOwner;
    } else {
      throw std::runtime_error("GameObject no longer exists.");
    }
    m_Instance->OnCreate();

    m_LastCompileTime = std::filesystem::last_write_time(GetScriptPath());
  } catch (const std::exception& e) {
    compiled = false;
    UnloadScript();
    throw;
  }

  compiled = true;
  return "";
}

void ScriptComponent::Update(float deltaTime) {
  if (compiled)
    m_Instance->OnUpdate(deltaTime);
}