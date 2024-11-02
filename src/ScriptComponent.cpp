#include "ScriptComponent.h"
#include "ScriptBase.h"

#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

ScriptComponent::~ScriptComponent()
{
    UnloadScript();
}

void ScriptComponent::LoadScript(const std::string& scriptName)
{
    m_ScriptName = scriptName;
    CompileAndLoad();
}

void ScriptComponent::ReloadIfNeeded()
{
    auto scriptPath = GetScriptPath();
    
    if (!std::filesystem::exists(scriptPath))
    {
        // Handle error
        return;
    }

    auto currentTime = std::filesystem::last_write_time(scriptPath);

    if (currentTime > m_LastCompileTime)
    {
        CompileAndLoad();
    }
}

std::string ScriptComponent::GetScriptPath() const
{
    return "scripts/" + m_ScriptName + ".cpp";
}

std::string ScriptComponent::GetCompiledPath() const
{
    #ifdef _WIN32
        return "scripts/compiled/" + m_ScriptName + ".dll";
    #else
        return "scripts/compiled/" + m_ScriptName + ".so";
    #endif
}

void ScriptComponent::CompileScript()
{
    std::filesystem::create_directories("scripts/compiled");

    #ifdef _WIN32 //I... don't really want to think about this right now. 
        std::string command = "cl.exe /LD /I../../include /std:c++17" +
                                GetScriptPath() + " /Fe" + GetCompiledPath();
    #else
        std::string command = "g++ -fPIC -shared -std=c++17 -Iinclude -O3 " + 
                                GetScriptPath() + " -o " + GetCompiledPath();
    #endif

    //std::cout << "pwd:";
    //system("pwd");

    //std::cerr << command << std::endl;

    int result = system(command.c_str());

    if (result != 0)
    {
        //Handle error
        //std::cerr << "ERROR DURING COMPILATION";
    } else
    {
        //std::cout << "COMPILED SUCCESSFULLY\n";
    }
}

void ScriptComponent::UnloadScript()
{
    if (m_Instance)
    {
        using DestroyScriptFn = void (*)(ScriptBase*);

        #ifdef _WIN32
            auto destroyScript = (DestroyScriptFn)GetProcAddress((HMODULE)m_DllHandle, "DestroyScript");
        #else
            auto destroyScript = (DestroyScriptFn)dlsym(m_DllHandle, "DestroyScript");
        #endif

        if (destroyScript)
        {
            m_Instance->OnDestroy();
            destroyScript(m_Instance);
        }

        m_Instance = nullptr;
    }

    if (m_DllHandle)
    {
        #ifdef _WIN32
            FreeLibrary((HMODULE)m_DllHandle);
        #else
            dlclose(m_DllHandle);
        #endif

        m_DllHandle = nullptr;
    }
}

void ScriptComponent::CompileAndLoad()
{
    try
    {
        //std::cout << "start CompileScript()\n";
        CompileScript();

        //std::cout << "start UnloadScript()\n";
        UnloadScript();

        //std::cout << "start dlopen()\n";
        #ifdef _WIN32
            m_DllHandle = LoadLibrary(GetCompiledPath().c_str());
        #else
            m_DllHandle = dlopen(GetCompiledPath().c_str(), RTLD_NOW);
        #endif

        //std::cout << "check dlopen(): GetCompiledPath:" + GetCompiledPath();
        if (!m_DllHandle)
        {
            //std::cerr << "dlopen error: " << dlerror() << std::endl;
            throw std::runtime_error("Failed to load script library: " + m_ScriptName + "(" + GetCompiledPath() + ")");
        }

        #ifdef _WIN32
            auto createScript = (ScriptBase* (*)())GetProcAddress((HMODULE)m_DllHandle, "CreateScript");
        #else
            auto createScript = (ScriptBase* (*)())dlsym(m_DllHandle, "CreateScript");
        #endif

        if (!createScript)
        {
            throw std::runtime_error("Failed to get CreateScript functon");
        }

        m_Instance = createScript();
        if (!m_Instance)
        {
            throw std::runtime_error("Failed to create script instance");
        }

        m_Instance->m_gameObject = m_owner;
        m_Instance->OnCreate();

        m_LastCompileTime = std::filesystem::last_write_time(GetScriptPath());
    }
    catch(const std::exception& e)
    {
        UnloadScript();
        throw;
    }
    
}

void ScriptComponent::Update(float deltaTime)
{
    m_Instance->OnUpdate(deltaTime); 
}