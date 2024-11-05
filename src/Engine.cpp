#include "Engine.h"

#include "MeshComponent.h"

// Initialize engine variables
Engine::Engine()
    : m_renderer(nullptr), m_sceneManager(nullptr), m_isRunning(false) {}

Engine::~Engine() {
  Shutdown();
}

// Set up renderer, scene manager, and demo GameObjects
bool Engine::Initialize() {
  m_renderer = std::make_shared<Renderer>();
  m_sceneManager = std::make_shared<SceneManager>();

  if (!m_renderer->Initialize()) {
    std::cerr << "renderer\n";
    return false;
  }

  m_editor = std::make_shared<Editor>();
  if (!m_editor->Initialize(m_renderer->GetWindow(), m_sceneManager)) {
    std::cerr << "editor\n";
    return false;
  }

  m_renderer->SetEditor(m_editor);

  m_sceneManager->CreateScene();

  // Add cube object to the scene
  auto cube = std::make_shared<GameObject>("Cube");
  cube->Initialize();
  cube->GetComponent<TransformComponent>()->SetPosition(
      glm::vec3(2.5f, 0.0f, 5.0f));
  auto scriptComponent = cube->AddComponent<ScriptComponent>();
  cube->GetComponent<ScriptComponent>()->LoadScript("PlayerController");
  // This is, of course, just for the demo scene. Will need to implement a way
  // to scan the scripts in the assets and use a selector.
  cube->AddComponent<MeshComponent>();
  m_sceneManager->AddObjectToCurrentScene(cube);

  // Add plane object to the scene
  auto plane = std::make_shared<GameObject>("Plane");
  plane->Initialize();
  plane->GetComponent<TransformComponent>()->SetPosition(
      glm::vec3(0.0f, -2.0f, 0.0f));
  plane->GetComponent<TransformComponent>()->SetScale(
      glm::vec3(25.0f, 1.0f, 25.0f));
  plane->AddComponent<MeshComponent>(std::make_shared<Mesh>(MeshType::Plane));
  m_sceneManager->AddObjectToCurrentScene(plane);

  auto light = std::make_shared<GameObject>("Light");
  light->Initialize();
  light->AddComponent<LightComponent>();
  m_sceneManager->AddObjectToCurrentScene(light);

  m_isRunning = true;

  return true;
}

// Main engine loop for updating and rendering the scene
void Engine::Run() {
  while (m_isRunning) {
    // Update with a fixed time step
    m_sceneManager->UpdateCurrentScene(0.016f);
    m_renderer->Render(m_sceneManager->GetScene());  // Render current scene
    m_isRunning = !m_renderer->ShouldClose();
  }
}

// Clean up renderer and scene manager
void Engine::Shutdown() {
  if (m_editor) {
    // m_editor->Shutdown();
    // delete m_editor;
    m_editor = nullptr;
  }
  if (m_renderer) {
    // m_renderer->Shutdown();
    // delete m_renderer;
    m_renderer = nullptr;
  }
  if (m_sceneManager) {
    m_sceneManager->DestroyScene();
    // delete m_sceneManager;
    m_sceneManager = nullptr;
  }
}