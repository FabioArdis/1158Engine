#include <iostream>

#include "Engine.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "SceneManager.h"

int main() {
  Engine engine;

  if (engine.Initialize())
    engine.Run();

  engine.Shutdown();

  return 0;
}