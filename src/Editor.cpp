// This file uses ImGui.
// Copyright (c) 2014-2024 Omar Cornut
// License: MIT (included in the LICENSE file)

// This file uses Lucide.
// Copyright 2013-2022 Cole Bemis
// License: ISC (included in the LICENSE file)

#include "Editor.h"

#include "IconsLucide.h"
#include "MeshComponent.h"
#include "ScriptBase.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <linux/limits.h>
#include <unistd.h>
#endif
#include <array>
#include <string>

Editor::Editor()
    : m_framebuffer(0),
      m_viewportColorTexture(0),
      m_depthStencilRBO(0),
      m_viewportSize(800, 600),
      m_viewportFocused(false),
      m_selectedObject(nullptr) {}

Editor::~Editor() {
  Shutdown();
}

bool Editor::Initialize(GLFWwindow* window,
                        const std::shared_ptr<SceneManager>& sceneManager) {
  m_sceneManager = sceneManager;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  io.Fonts->AddFontDefault();
  static constexpr std::array<ImWchar, 3> icons_ranges = {ICON_MIN_LC, ICON_MAX_LC,
                                                      0};
  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = 16.0f;
  icons_config.GlyphOffset.y += 8.0f;
  io.Fonts->AddFontFromFileTTF("fonts/lucide.ttf", 24.0f, &icons_config,
                               icons_ranges.data());

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 0.0f;

  SetupFramebuffer();

  try {
    m_rootPath = std::filesystem::current_path().string();
    m_currentPath = m_rootPath;
    UpdateDirectoryContents();
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Filesystem error during initialization: " << e.what() << "\n";
    m_rootPath = std::filesystem::current_path().string();
    m_currentPath = m_rootPath;
    UpdateDirectoryContents();
  }

  m_notificationManager = std::make_shared<NotificationManager>();

  return true;
}

void Editor::Shutdown() const {
  if (m_framebuffer != 0) {
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteTextures(1, &m_viewportColorTexture);
    glDeleteRenderbuffers(1, &m_depthStencilRBO);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Editor::Begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  Editor::SetupDockspace();
}

void Editor::End() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Render(const std::shared_ptr<Scene>& scene) {
  RenderMenuBar(scene);

  // ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;

  ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"),
                             ImGuiCond_FirstUseEver);
  RenderSceneHierarchy(scene);

  ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"),
                             ImGuiCond_FirstUseEver);
  RenderViewport(scene);

  ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"),
                             ImGuiCond_FirstUseEver);
  RenderPropertiesPanel();

  ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"),
                             ImGuiCond_FirstUseEver);
  RenderAssetsPanel();

  m_notificationManager->RenderNotifications();

  ImGui::End();
}

void Editor::SetupDockspace() {
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
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
  if (first_time) {
    first_time = false;

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id,
                              dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

    // Split layout between the main docks
    ImGuiID dock_main = dockspace_id;
    ImGuiID dock_id_left_right = ImGui::DockBuilderSplitNode(
        dock_main, ImGuiDir_Left,
        (viewport->WorkSize.x - viewport->WorkSize.x / 4) /
            viewport->WorkSize.x,
        nullptr, &dock_main);
    ImGuiID dock_id_left = dock_id_left_right;
    ImGuiID dock_id_right = dock_main;

    // Split left side into top and bottom
    ImGuiID dock_id_top = ImGui::DockBuilderSplitNode(
        dock_id_left, ImGuiDir_Up,
        (viewport->WorkSize.y / 2) / viewport->WorkSize.y, nullptr,
        &dock_id_left);

    // Split top into Scene Hierarchy and Viewport
    ImGuiID dock_id_scene = ImGui::DockBuilderSplitNode(
        dock_id_top, ImGuiDir_Left,
        (viewport->WorkSize.x / 2) / viewport->WorkSize.x, nullptr,
        &dock_id_top);

    // Dock windows
    ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_id_scene);
    ImGui::DockBuilderDockWindow("Viewport", dock_id_top);
    ImGui::DockBuilderDockWindow("Properties", dock_id_right);
    ImGui::DockBuilderDockWindow("Assets", dock_id_left);

    ImGui::DockBuilderFinish(dockspace_id);
  }
}

void Editor::SetupFramebuffer() {
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  glGenTextures(1, &m_viewportColorTexture);
  glBindTexture(GL_TEXTURE_2D, m_viewportColorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(m_viewportSize.x),
               static_cast<GLsizei>(m_viewportSize.y), 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_viewportColorTexture, 0);

  glGenRenderbuffers(1, &m_depthStencilRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        static_cast<GLsizei>(m_viewportSize.x),
                        static_cast<GLsizei>(m_viewportSize.y));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, m_depthStencilRBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    // TODO: Handle error status
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::UpdateFramebuffer() const {
  glBindTexture(GL_TEXTURE_2D, m_viewportColorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(m_viewportSize.x),
               static_cast<GLsizei>(m_viewportSize.y), 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);

  glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                        static_cast<GLsizei>(m_viewportSize.x),
                        static_cast<GLsizei>(m_viewportSize.y));
}

void Editor::RenderViewport(const std::shared_ptr<Scene>& scene) {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Viewport");

  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  m_viewportFocused = ImGui::IsWindowFocused();

  float desiredAspectRatio = 16.0f / 9.0f;
  float newWidth = viewportPanelSize.y * desiredAspectRatio;
  float newHeight = viewportPanelSize.y;

  if (newWidth > viewportPanelSize.x) {
    newWidth = viewportPanelSize.x;
    newHeight = newWidth / desiredAspectRatio;
  }

  if (newWidth != m_viewportSize.x || newHeight != m_viewportSize.y) {
    m_viewportSize = ImVec2(newWidth, newHeight);
    UpdateFramebuffer();
  }

  // Use this instead if you want to fill the dock
  /* if (viewportPanelSize.x > 0 && viewportPanelSize.y > 0 &&
        (viewportPanelSize.x != m_viewportSize.x || viewportPanelSize.y !=
    m_viewportSize.y))
    {
        m_viewportSize = viewportPanelSize;
        UpdateFramebuffer();
    } */

  ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(m_viewportColorTexture)), m_viewportSize,
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();
  ImGui::PopStyleVar();
}

void Editor::RenderSceneHierarchy(const std::shared_ptr<Scene>& scene) {
  ImGui::Begin("Scene Hierarchy");

  for (auto& object : scene->GetGameObjects()) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (m_selectedObject == object) {
      flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool opened = ImGui::TreeNodeEx(
        (object->GetName() + "##" + std::to_string(object->GetID())).c_str(),
        flags);
    std::string popupId = "item context menu" + std::to_string(object->GetID());

    if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
      m_selectedObject = object;
    }

    if (ImGui::IsItemClicked(1)) {
      ImGui::OpenPopup(popupId.c_str());
    }

    if (ImGui::BeginPopup(popupId.c_str())) {
      if (ImGui::MenuItem("Delete")) {
        scene->RemoveGameObject(object);
        m_selectedObject = nullptr;
      }
      ImGui::EndPopup();
    }

    if (opened)
      ImGui::TreePop();
  }

  ImGui::End();
}

void Editor::RenderPropertiesPanel() const {
  ImGui::Begin("Properties");

  if (m_selectedObject != nullptr) {
    std::string popupId =
        "add component menu" + std::to_string(m_selectedObject->GetID());

    //ImGui::Text("Selected: %s", m_selectedObject->GetName().c_str());

    std::string strName = m_selectedObject->GetName();
    constexpr size_t bufferSize = 256;
    std::array<char, bufferSize> buffer = {};

    strName.copy(buffer.data(), bufferSize - 1);
    buffer[std::min(strName.length(), bufferSize - 1)] = '\0';  // Ensure null-termination

    if (ImGui::InputText((" Name##" + std::to_string(m_selectedObject->GetID())).c_str(), buffer.data(), bufferSize)) {
      m_selectedObject->SetName(buffer.data());
    }

    std::string errorMessage;

    if (auto* transform =
            m_selectedObject->GetComponent<TransformComponent>()) {
      if (ImGui::CollapsingHeader("Transform Component")) {
        glm::vec3 position = transform->GetPosition();
        glm::vec3 rotation = transform->GetRotation();
        glm::vec3 scale = transform->GetScale();

        if (ImGui::DragFloat3("Position", &position[0], 0.1f)) {
          transform->SetPosition(position);
        }
        if (ImGui::DragFloat3("Rotation", &rotation[0], 0.1f)) {
          transform->SetRotation(rotation);
        }
        if (ImGui::DragFloat3("Scale", &scale[0], 0.1f)) {
          transform->SetScale(scale);
        }
      }
    }

    if (auto* mesh = m_selectedObject->GetComponent<MeshComponent>()) {
      if (ImGui::CollapsingHeader("Mesh Component")) {
        MeshType currentMeshType = mesh->GetMeshType();
        const char* currentMeshName =
            MeshTypeNames[static_cast<int>(currentMeshType)].data();
        if (ImGui::BeginCombo("Mesh", currentMeshName)) {
          for (int i = 0; i < static_cast<int>(MeshType::Count) - 1; ++i) {
            auto meshType = static_cast<MeshType>(i);
            bool isSelected = (meshType == currentMeshType);

            if (ImGui::Selectable(MeshTypeNames[i].data(), isSelected)) {
              mesh->ClearMesh();
              mesh->SetMesh(std::make_shared<Mesh>(meshType));
            }

            if (isSelected) {
              ImGui::SetItemDefaultFocus();
            }
          }

          ImGui::EndCombo();
        }
      }
    }

    if (auto* light = m_selectedObject->GetComponent<LightComponent>()) {
      if (ImGui::CollapsingHeader("Light Component")) {
        glm::vec3 color = light->GetColor();
        float intensity = light->GetIntensity();
        float range = light->GetRange();

        if (ImGui::ColorPicker3("ColorPicker", &color[0])) {
          light->SetColor(color);
        }
        if (ImGui::DragFloat("Intensity", &intensity, 0.1f)) {
          light->SetIntensity(intensity);
        }
        if (ImGui::DragFloat("Range", &range, 0.1f)) {
          light->SetRange(range);
        }
      }
    }

    if (auto* script = m_selectedObject->GetComponent<ScriptComponent>()) {
      std::string headerName = "Script Component (" + script->GetName() + ")";
      if (ImGui::CollapsingHeader(headerName.c_str())) {
        auto* instance = script->GetScriptInstance();

        if (instance) {
          for (const auto& [name, type] : instance->GetProperties()) {
            void* ptr = instance->GetPropertyPtr(name);

            switch (type) {
              case PropertyType::Float:
                ImGui::DragFloat(name.c_str(), static_cast<float*>(ptr), 0.1f);
              break;
              case PropertyType::Int:
                ImGui::DragInt(name.c_str(), static_cast<int*>(ptr));
              break;
              case PropertyType::Bool:
                ImGui::Checkbox(name.c_str(), static_cast<bool*>(ptr));
              break;
              case PropertyType::Vec2:
                ImGui::DragFloat2(name.c_str(), &static_cast<glm::vec2*>(ptr)->x);
              break;
              case PropertyType::Vec3:
                ImGui::DragFloat3(name.c_str(), &static_cast<glm::vec3*>(ptr)->x);
              break;
              case PropertyType::Vec4:
                ImGui::DragFloat4(name.c_str(), &static_cast<glm::vec4*>(ptr)->x);
              break;
              case PropertyType::String: {
                auto* str = static_cast<std::string*>(ptr);
                constexpr size_t bufferSize = 256;
                std::array<char, bufferSize> buffer = {};

                std::copy_n(str->begin(),
                            std::min(str->size(), buffer.size() - 1),
                            buffer.data());

                if (ImGui::InputText(name.c_str(), buffer.data(), bufferSize)) {
                  *str = buffer.data();
                }

                break;
              }
            }
          }
        } else {
          ImGui::Text(("Script " + script->GetName() + " is not compiled.").c_str());
        }


        if (ImGui::Button("Reload Script")) {
          errorMessage = script->ReloadIfNeeded();
          if (!errorMessage.empty()) {
            m_notificationManager->AddNotification("Could not reload: " + errorMessage);
          }
        }
      }
    }

    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup(popupId.c_str());
    }

    if (ImGui::BeginPopup(popupId.c_str())) {
      if (ImGui::MenuItem("LightComponent")) {
        m_selectedObject->AddComponent<LightComponent>();
      }
      if (ImGui::MenuItem("MeshComponent")) {
        m_selectedObject->AddComponent<MeshComponent>(
            std::make_shared<Mesh>(MeshType::Cube));
      }
      ImGui::EndPopup();
    }
  }

  ImGui::End();
}

void Editor::RenderMenuBar(const std::shared_ptr<Scene>& scene) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Scene")) {
        m_sceneManager->DestroyScene();
        m_sceneManager->CreateScene();
      }
      if (ImGui::MenuItem("Open Scene...")) {
        // TODO: Implement scene loading.
      }
      if (ImGui::MenuItem("Save Scene")) {
        // TODO: Implement scene saving logic.
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Exit")) {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), 1);
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {
        // TODO: Implement an action buffer for undo
      }
      if (ImGui::MenuItem("Redo", "CTRL+Y")) {
        // TODO: Implement an action buffer for redo
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Add")) {
      if (ImGui::MenuItem("Empty GameObject")) {
        auto gameObject = std::make_shared<GameObject>(
            scene->GenerateUniqueName("EmptyGameObject"));
        gameObject->Initialize();
        scene->AddGameObject(gameObject);
        m_selectedObject = gameObject;
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Cube")) {
        auto gameObject =
            std::make_shared<GameObject>(scene->GenerateUniqueName("Cube"));
        gameObject->Initialize();
        gameObject->AddComponent<MeshComponent>(
            std::make_shared<Mesh>(MeshType::Cube));
        scene->AddGameObject(gameObject);
        m_selectedObject = gameObject;
      }
      if (ImGui::MenuItem("Plane")) {
        auto gameObject =
            std::make_shared<GameObject>(scene->GenerateUniqueName("Plane"));
        gameObject->Initialize();
        gameObject->AddComponent<MeshComponent>(
            std::make_shared<Mesh>(MeshType::Plane));
        scene->AddGameObject(gameObject);
        m_selectedObject = gameObject;
      }
      if (ImGui::MenuItem("Capsule")) {
        auto gameObject =
            std::make_shared<GameObject>(scene->GenerateUniqueName("Capsule"));
        gameObject->Initialize();
        gameObject->AddComponent<MeshComponent>(
            std::make_shared<Mesh>(MeshType::Capsule));
        scene->AddGameObject(gameObject);
        m_selectedObject = gameObject;
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Point Light")) {
        auto gameObject = std::make_shared<GameObject>(
            scene->GenerateUniqueName("PointLight"));
        gameObject->Initialize();
        gameObject->AddComponent<LightComponent>();
        scene->AddGameObject(gameObject);
        m_selectedObject = gameObject;
      }

      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }
}

void Editor::RenderAssetsPanel() {
  ImGui::Begin("Assets");

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

  bool canGoUp = m_currentPath != m_rootPath;
  if (!canGoUp)
    ImGui::BeginDisabled();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

  if (ImGui::Button(ICON_LC_ARROW_LEFT)) {
    std::filesystem::path currentPath(m_currentPath);
    if (currentPath.has_parent_path()) {
      std::string parentPath = currentPath.parent_path().string();

      if (parentPath.find(m_rootPath) == 0) {
        m_currentPath = parentPath;
        UpdateDirectoryContents();
      }
    }
  }

  ImGui::PopStyleColor();

  if (!canGoUp)
    ImGui::EndDisabled();

  ImGui::SameLine();

  std::string displayPath = m_currentPath;

  ImGui::Text("%s", displayPath.c_str());

  ImGui::PopStyleVar();
  ImGui::Separator();

  float cellPadding = 10.0f;
  float iconSize = 50.0f;
  float cellSize = iconSize + cellPadding * 2;
  float panelWidth = ImGui::GetContentRegionAvail().x;
  int columnsCount = static_cast<int>(panelWidth / cellSize);
  if (columnsCount < 1)
    columnsCount = 1;

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                      ImVec2(cellPadding, cellPadding));

  ImGui::Columns(columnsCount, nullptr, false);

  for (size_t i = 0; i < m_currentDirectoryContents.size(); i++) {
    const auto& entry = m_currentDirectoryContents[i];
    ImGui::BeginGroup();

    ImGui::PushID(static_cast<int>(i));
    bool isSelected = false;

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

    if (entry.isDirectory) {
      if (ImGui::Button(ICON_LC_FOLDER, ImVec2(iconSize, iconSize))) {
        m_currentPath = entry.fullPath;
        UpdateDirectoryContents();
      }
    } else {
      std::string ext = std::filesystem::path(entry.name).extension().string();
      const char* icon = ICON_LC_FILE;  // Default

      if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
        icon = ICON_LC_IMAGE;
      else if (ext == ".obj" || ext == ".fbx" || ext == ".gltf")
        icon = ICON_LC_BOX;
      else if (ext == ".wav" || ext == ".mp3")
        icon = ICON_LC_FILE_MUSIC;
      else if (ext == ".h" || ext == ".hpp")
        icon = ICON_LC_LIBRARY_BIG;
      else if (ext == ".c" || ext == ".cpp")
        icon = ICON_LC_FILE_CODE;

      if (ImGui::Button(icon, ImVec2(iconSize, iconSize))) {
        isSelected = true;
      }
    }

    ImGui::PopStyleColor();

    ImVec2 labelSize = ImGui::CalcTextSize(entry.name.c_str());
    float labelWidth = cellSize - cellPadding * 2;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX());
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + labelWidth);

    if (labelSize.x > labelWidth) {
      if (!isSelected) {
        if (entry.name.size() > 14) {
          std::string truncated = entry.name.substr(0, 14) + "...";
          ImGui::TextWrapped("%s", truncated.c_str());
        } else {
          ImGui::TextWrapped("%s", entry.name.c_str());
        }
      } else {
        ImGui::TextWrapped("%s", entry.name.c_str());
        // TODO: it resets after release, fix later
      }

    } else {
      ImGui::TextWrapped("%s", entry.name.c_str());
    }

    ImGui::PopTextWrapPos();

    ImGui::EndGroup();
    ImGui::NextColumn();
    ImGui::PopID();
  }

  ImGui::Columns(1);
  ImGui::PopStyleVar();
  ImGui::End();
}

void Editor::UpdateDirectoryContents() {
  m_currentDirectoryContents.clear();

  try {
    for (const auto& entry :
         std::filesystem::directory_iterator(m_currentPath)) {
      if (entry.path().filename().string()[0] == '.')
        continue;
      FileEntry fileEntry;
      fileEntry.name = entry.path().filename().string();
      fileEntry.isDirectory = entry.is_directory();
      fileEntry.fullPath = entry.path().string();
      m_currentDirectoryContents.push_back(fileEntry);
    }

    // Sorting
    std::sort(m_currentDirectoryContents.begin(),
              m_currentDirectoryContents.end(),
              [](const FileEntry& a, const FileEntry& b) {
                if (a.isDirectory != b.isDirectory) {
                  return a.isDirectory > b.isDirectory;
                }
                return a.name < b.name;
              });
  } catch (const std::filesystem::filesystem_error& e) {
    // TODO: Handle the errors
    std::cerr << "Error accessing directory: " << e.what() << std::endl;
  }
}
