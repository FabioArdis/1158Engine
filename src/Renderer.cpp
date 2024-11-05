// This file uses ImGui.
// Copyright (c) 2014-2024 Omar Cornut
// License: MIT (included in the LICENSE file)

#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "MeshComponent.h"
#include "TransformComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

std::string FormatFPS(double fps, int decimalPlaces) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(decimalPlaces) << fps;
  return stream.str();
}

Renderer::Renderer()
    : m_window(nullptr),
      m_camera(nullptr),
      m_inputManager(nullptr),
      m_shaderManager(nullptr),
      m_textRenderer(nullptr),
      m_lastTime(0.0),
      m_nbFrames(0),
      m_fps(0.0),
      m_frameTime(0.0) {

  // Initialize lights
  m_lights.emplace_back(
      Light(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.2f));
  m_lights.emplace_back(
      Light(glm::vec3(5.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.2f));
  m_lights.emplace_back(
      Light(glm::vec3(5.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.2f));
  m_lights.emplace_back(
      Light(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 1.0f), 0.2f));
}

Renderer::~Renderer() {
  Shutdown();
}

bool Renderer::Initialize() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }

  // Set GLFW window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  // Create window
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  m_window = glfwCreateWindow(mode->width, mode->height, "1158Engine", nullptr,
                              nullptr);

  // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); Will
  // probably reuse this when we will catch the cursor inside the viewport

  if (!m_window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_window);

  // Load OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
  }

  // Initialize shader and input managers
  m_shaderManager = std::make_shared<ShaderManager>();
  m_shaderManager->LoadShader("default", "shaders/basic.vert",
                              "shaders/basic.frag");
  m_shaderManager->LoadShader("font", "shaders/font.vert", "shaders/font.frag");
  m_inputManager = std::make_shared<InputManager>(m_window);
  m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), -90.f, 0.0f);

  // Initialize text renderer
  m_textRenderer = std::make_unique<TextRenderer>(m_shaderManager);
  if (!m_textRenderer->Initialize("fonts/OpenSans-Regular.ttf", 24)) {
    std::cerr << "Failed to initialize text renderer\n";
    return false;
  }

  glEnable(GL_DEPTH_TEST);

  // Uncomment for blending
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_lastTime = glfwGetTime();
  m_nbFrames = 0;
  m_fps = 0.0;

  // Uncomment to disable vsync
  // glfwSwapInterval(0);

  return true;
}

void Renderer::Shutdown() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Renderer::SetCamera(std::unique_ptr<Camera> camera) {
  m_camera = std::move(camera);
}

void Renderer::SetLights(const std::vector<Light>& lights) {
  m_lights = lights;
  m_shaderManager->UseShader("default");

  // Set light properties
  for (size_t i = 0; i < lights.size(); ++i) {
    std::string uniformName = "lights[" + std::to_string(i) + "]";

    m_shaderManager->SetVector3((uniformName + ".position").c_str(),
                                lights[i].GetPosition());

    m_shaderManager->SetVector3((uniformName + ".color").c_str(),
                                lights[i].GetColor());

    m_shaderManager->SetFloat((uniformName + ".intensity").c_str(),
                              lights[i].GetIntensity());
  }
}

void Renderer::RenderObject(std::shared_ptr<GameObject> object) {
  auto* meshComponent = object->GetComponent<MeshComponent>();

  if (meshComponent != nullptr) {
    auto* transformComponent = object->GetComponent<TransformComponent>();

    if (transformComponent != nullptr) {
      glm::mat4 model = transformComponent->GetTransformMatrix();
      m_shaderManager->SetMatrix4("model", model);

      std::shared_ptr<Mesh> mesh = meshComponent->GetMesh();

      if (mesh != nullptr) {
        // SetLights(m_lights); unused, we are using the new system.
        mesh->Draw();
      }
    }
  }
}

bool Renderer::ShouldClose() {
  return glfwWindowShouldClose(m_window);
}

void Renderer::Render(std::shared_ptr<Scene> scene) {
  CalculateFPS();

  if (m_editor != nullptr) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_editor->GetFramebuffer());
    glViewport(0, 0, m_editor->GetViewPortSize().x,
               m_editor->GetViewPortSize().y);
  }

  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_inputManager->PollEvents();

  if (m_camera != nullptr) {
    // Camera movement
    if (m_editor->IsViewportFocused()) {
      if (m_inputManager->IsKeyPressed(GLFW_KEY_W))
        m_camera->MoveForward();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_S))
        m_camera->MoveBackward();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_A))
        m_camera->MoveLeft();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_D))
        m_camera->MoveRight();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_Q))
        m_camera->RotateLeft();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_E))
        m_camera->RotateRight();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
        m_camera->MoveDown();
      if (m_inputManager->IsKeyPressed(GLFW_KEY_SPACE))
        m_camera->MoveUp();
    }

    // Prepare matrices
    int screenWidth, screenHeight;
    glfwGetWindowSize(m_window, &screenWidth, &screenHeight);
    glm::mat4 view = m_camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
        0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    m_shaderManager->UseShader("default");
    m_shaderManager->SetMatrix4("view", view);
    m_shaderManager->SetMatrix4("projection", projection);
    m_shaderManager->SetMatrix4("model", model);

    UpdateLights(scene);

    for (auto& object : scene->GetGameObjects()) {
      RenderObject(object);
    }

    // Render text overlay
    {
      glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth),
                                        0.0f, static_cast<float>(screenHeight));
      m_textRenderer->SetProjection(projection);

      glDisable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      std::string fpsText = FormatFPS(m_fps, 1);
      m_textRenderer->RenderText(fpsText + " FPS", 25.0f, screenHeight - 50.0f,
                                 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

      glDisable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
    }
  }

  if (m_editor != nullptr) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_editor->Begin();
    m_editor->Render(scene);
    Editor::End();
  }

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void Renderer::CalculateFPS() {
  double currentTime = glfwGetTime();
  m_frameTime = currentTime - m_lastTime;
  m_nbFrames++;
  if (m_frameTime >= 1.0) {  // Update FPS every second
    m_fps = static_cast<double>(m_nbFrames) / m_frameTime;
    m_nbFrames = 0;
    m_lastTime = currentTime;
  }
}

void Renderer::UpdateLights(std::shared_ptr<Scene> scene) {
  const unsigned int MAX_LIGHTS =
      m_shaderManager->GetUniformLocation("MAX_LIGHTS");
  std::vector<LightComponent*> sceneLights;

  for (auto& gameObject : scene->GetGameObjects()) {
    if (auto* lightComp = gameObject->GetComponent<LightComponent>()) {
      sceneLights.push_back(lightComp);
    }
  }

  m_shaderManager->UseShader("default");

  unsigned int numLights =
      std::min(static_cast<unsigned int>(sceneLights.size()), MAX_LIGHTS);

  m_shaderManager->SetInt("numLights", numLights);

  for (unsigned int i = 0; i < numLights; ++i) {
    std::string uniformName = "lights[" + std::to_string(i) + "]";
    auto* light = sceneLights[i];

    m_shaderManager->SetVector3((uniformName + ".position").c_str(),
                                light->GetPosition());
    m_shaderManager->SetVector3((uniformName + ".color").c_str(),
                                light->GetColor());
    m_shaderManager->SetFloat((uniformName + ".intensity").c_str(),
                              light->GetIntensity());
  }
}
