#pragma once

// This file uses ImGui.
// Copyright (c) 2014-2024 Omar Cornut
// License: MIT (included in the LICENSE file)

#include "SceneManager.h"
#include "Scene.h"
#include "imgui.h"
#include <glad/glad.h>
#include <string>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <vector>

class Editor
{
public:
    Editor();
    ~Editor();

    bool Initialize(GLFWwindow* window, SceneManager* sceneManager);
    void Shutdown();
    void Begin();
    void End();
    void Render(Scene* scene);

    const ImVec2 GetViewPortSize() const { return m_viewportSize; }
    GLuint GetViewportTexture() const { return m_viewportColorTexture; }
    GLuint GetFramebuffer() const { return m_framebuffer; }
    bool IsViewportFocused() const { return m_viewportFocused; }

private:
    struct FileEntry
    {
        std::string name;
        bool isDirectory;
        std::string fullPath;
    };

    std::string m_rootPath;
    std::string m_currentPath;
    std::vector<FileEntry> m_currentDirectoryContents;

    void SetupDockspace();
    void SetupFramebuffer();
    
    void UpdateFramebuffer();

    void RenderViewport(Scene* scene);
    void RenderSceneHierarchy(Scene* scene);
    void RenderPropertiesPanel();
    void RenderMenuBar(Scene* scene);
    void RenderAssetsPanel();

    void UpdateDirectoryContents();

    GLuint m_framebuffer;
    GLuint m_viewportColorTexture;
    GLuint m_depthStencilRBO;
    ImVec2  m_viewportSize;
    bool m_viewportFocused;
    GameObject* m_selectedObject;
    GameObject* m_renamingObject;

    SceneManager* m_sceneManager;
};