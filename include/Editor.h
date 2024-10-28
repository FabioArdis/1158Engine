#pragma once

// This file uses ImGui.
// Copyright (c) 2014-2024 Omar Cornut
// License: MIT (included in the LICENSE file)

#include "Scene.h"
#include "imgui.h"
#include <glad/glad.h>
#include <string>
#include <GLFW/glfw3.h>

class Editor
{
public:
    Editor();
    ~Editor();

    bool Initialize(GLFWwindow* window);
    void Shutdown();
    void Begin();
    void End();
    void Render(Scene* scene);

    const ImVec2 GetViewPortSize() const { return m_viewportSize; }
    GLuint GetViewportTexture() const { return m_viewportColorTexture; }
    GLuint GetFramebuffer() const { return m_framebuffer; }
    bool IsViewportFocused() const { return m_viewportFocused; }

private:
    void SetupDockspace();
    void SetupFramebuffer();
    void UpdateFramebuffer();
    void RenderViewport(Scene* scene);
    void RenderSceneHierarchy(Scene* scene);
    void RenderPropertiesPanel();
    void RenderMenuBar(Scene* scene);

    GLuint m_framebuffer;
    GLuint m_viewportColorTexture;
    GLuint m_depthStencilRBO;
    ImVec2  m_viewportSize;
    bool m_viewportFocused;
    GameObject* m_selectedObject;
};