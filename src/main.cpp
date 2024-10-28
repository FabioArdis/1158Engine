#include "Engine.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "SceneManager.h"
#include <iostream>

int main()
{
	Engine engine;

	if (engine.Initialize())
		engine.Run();
	
	engine.Shutdown();

	return 0;
}