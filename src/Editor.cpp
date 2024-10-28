// This file uses ImGui.
// Copyright (c) 2014-2024 Omar Cornut
// License: MIT (included in the LICENSE file)

#include "Editor.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MeshComponent.h"

Editor::Editor()
    : m_framebuffer(0), m_viewportColorTexture(0), 
      m_depthStencilRBO(0), m_viewportSize(800, 600),
      m_viewportFocused(false), m_selectedObject(nullptr)
{
}

Editor::~Editor()
{
    Shutdown();
}

bool Editor::Initialize(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;

    SetupFramebuffer();

    return true;
}

void Editor::Shutdown()
{
    if (m_framebuffer)
    {
        glDeleteFramebuffers(1, &m_framebuffer);
        glDeleteTextures(1, &m_viewportColorTexture);
        glDeleteRenderbuffers(1, &m_depthStencilRBO);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    SetupDockspace();
}

void Editor::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Render(Scene *scene)
{
    RenderMenuBar(scene);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
    RenderSceneHierarchy(scene);

    ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
    RenderViewport(scene);

    ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
    RenderPropertiesPanel();

    ImGui::End();
}

void Editor::SetupDockspace()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    static bool first_time = true;
    if (first_time)
    {
        first_time = false;

        ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);

        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.2f, viewport->WorkSize.y));
        ImGui::SetWindowPos(viewport->WorkPos);

        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.25f, viewport->WorkSize.y));
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.75f, viewport->WorkPos.y));

        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x * 0.55f, viewport->WorkSize.y));
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.2f, viewport->WorkPos.y));
    }
}

void Editor::SetupFramebuffer()
{
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glGenTextures(1, &m_viewportColorTexture);
    glBindTexture(GL_TEXTURE_2D, m_viewportColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewportSize.x, m_viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_viewportColorTexture, 0);

    glGenRenderbuffers(1, &m_depthStencilRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportSize.x, m_viewportSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // TODO: Handle error status
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::UpdateFramebuffer()
{
    glBindTexture(GL_TEXTURE_2D, m_viewportColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewportSize.x, m_viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportSize.x, m_viewportSize.y);
}

void Editor::RenderViewport(Scene *scene)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_viewportFocused = ImGui::IsWindowFocused();

    float desiredAspectRatio = 16.0f / 9.0f;
    float newWidth = viewportPanelSize.y * desiredAspectRatio;
    float newHeight = viewportPanelSize.y;

    if (newWidth > viewportPanelSize.x)
    {
        newWidth = viewportPanelSize.x;
        newHeight = newWidth / desiredAspectRatio;
    }

    if (newWidth != m_viewportSize.x || newHeight != m_viewportSize.y)
    {
        m_viewportSize = ImVec2(newWidth, newHeight);
        UpdateFramebuffer();
    }

    // Use this instead if you want to fill the dock
    /* if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0 &&
        (viewportPanelSize.x != m_viewportSize.x || viewportPanelSize.y != m_viewportSize.y))
    {
        m_viewportSize = viewportPanelSize;
        UpdateFramebuffer();
    } */

    ImGui::Image((ImTextureID)(intptr_t)m_viewportColorTexture, m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();
}

void Editor::RenderSceneHierarchy(Scene *scene)
{
    ImGui::Begin("Scene Hierarchy");

    for (auto & object : scene->GetGameObjects())
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (m_selectedObject == object)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool opened = ImGui::TreeNodeEx(object->GetName().c_str(), flags);

        if (ImGui::IsItemClicked())
        {
            m_selectedObject = object;
        }

        if (opened)
            ImGui::TreePop();
    }

    ImGui::End();
}

void Editor::RenderPropertiesPanel()
{
    ImGui::Begin("Properties");

    if (m_selectedObject)
    {
        ImGui::Text("Selected: %s", m_selectedObject->GetName().c_str());

        if (auto transform = m_selectedObject->GetComponent<TransformComponent>())
        {
            if (ImGui::CollapsingHeader("Transform"))
            {
                glm::vec3 position = transform->GetPosition();
                glm::vec3 rotation = transform->GetRotation();
                glm::vec3 scale = transform->GetScale();

                if (ImGui::DragFloat3("Position", &position[0], 0.1f))
                {
                    transform->SetPosition(position);
                }
                if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f))
                {
                    transform->SetRotation(rotation);
                }
                if (ImGui::DragFloat3("Scale", &scale[0], 0.1f))
                {
                    transform->SetScale(scale);
                }
            }
        }

        if (auto mesh = m_selectedObject->GetComponent<MeshComponent>())
        {
            if (ImGui::CollapsingHeader("Mesh"))
            {
                ImGui::Text("Mesh Component");
            }
        }
    }

    ImGui::End();
}

void Editor::RenderMenuBar(Scene* scene)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene")) {}
            if (ImGui::MenuItem("Open Scene...")) {}
            if (ImGui::MenuItem("Save Scene")) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add"))
        {
            if (ImGui::MenuItem("Empty GameObject")) {
                
                scene->AddGameObject(new GameObject("NewGameObject"));
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Cube")) {
                GameObject* gameObject = new GameObject("NewCube");
                gameObject->AddComponent<MeshComponent>(new Mesh(MeshType::Cube));
                scene->AddGameObject(gameObject);
            }
            if (ImGui::MenuItem("Plane")) {
                GameObject* gameObject = new GameObject("NewPlane");
                gameObject->AddComponent<MeshComponent>(new Mesh(MeshType::Plane));
                scene->AddGameObject(gameObject);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}
