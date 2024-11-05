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

class Editor {
 public:
  Editor();
  ~Editor();

  // We most likely won't need to use these constructors.
  Editor(const Editor&) = delete;
  Editor& operator=(const Editor&) = delete;
  Editor(Editor&&) = delete;
  Editor& operator=(Editor&&) = delete;

  /**
    * @brief Initializes the editor with the specified window and scene manager.
    * 
    * @param window Pointer to the GLFW window.
    * @param sceneManager Pointer to the SceneManager handling scenes.
    * @return True if initialization was successful, false otherwise.
    */
  bool Initialize(GLFWwindow* window,
                  std::shared_ptr<SceneManager> sceneManager);

  /**
    * @brief Shuts down the editor and releases resources.
    */
  void Shutdown();

  /**
    * @brief Begins the frame for ImGui rendering.
    */
  void Begin();

  /**
    * @brief Ends the frame for ImGui rendering.
    */
  static void End();

  /**
    * @brief Renders the editor UI and scene.
    * 
    * @param scene Pointer to the current scene to render in the editor.
    */
  void Render(std::shared_ptr<Scene> scene);

  [[nodiscard]] ImVec2 GetViewPortSize() const { return m_viewportSize; }

  [[nodiscard]] GLuint GetViewportTexture() const {
    return m_viewportColorTexture;
  }

  [[nodiscard]] GLuint GetFramebuffer() const { return m_framebuffer; }

  [[nodiscard]] bool IsViewportFocused() const { return m_viewportFocused; }

 private:
  struct FileEntry {
    std::string name;     /** Name of the file or directory. */
    bool isDirectory;     /** Whether the entry is a directory. */
    std::string fullPath; /** Full path to the file or directory. */
  };

  std::string m_rootPath;    /** Root directory path for assets. */
  std::string m_currentPath; /** Current directory path in the assets panel. */
  /** Contents of the current directory. */
  std::vector<FileEntry> m_currentDirectoryContents;

  static void SetupDockspace(); /** Configures ImGui dockspace layout. */

  void SetupFramebuffer(); /** Sets up the framebuffer for the viewport. */
  /** Updates the framebuffer if the viewport size changes. */
  void UpdateFramebuffer() const;

  /** Renders the scene in the viewport. */
  void RenderViewport(std::shared_ptr<Scene> scene);
  /** Renders the hierarchy of objects in the scene. */
  void RenderSceneHierarchy(std::shared_ptr<Scene> scene);
  /** Renders the properties panel for selected objects. */
  void RenderPropertiesPanel();
  /** Renders the menu bar for editor options. */
  void RenderMenuBar(std::shared_ptr<Scene> scene);
  void RenderAssetsPanel(); /** Renders the assets panel to browse files. */

  /** Updates the contents of the current directory. */
  void UpdateDirectoryContents();

  GLuint m_framebuffer{0}; /** Framebuffer object for the viewport. */
  /** Color texture attached to the framebuffer. */
  GLuint m_viewportColorTexture{0};
  GLuint m_depthStencilRBO{0}; /** Renderbuffer object for depth and stencil. */

  ImVec2 m_viewportSize; /** Size of the viewport in the editor. */
  /** Indicates if the viewport is currently focused. */
  bool m_viewportFocused{false};

  /** Currently selected game object. */
  std::shared_ptr<GameObject> m_selectedObject{nullptr};

  std::shared_ptr<SceneManager> m_sceneManager{nullptr};
};