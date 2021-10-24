#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "AssetManager.h"

int main()
{
	//initialize Asset manager
	AssetManager assetManager;

	//initialize game engine
	GameEngine gameEngine;

	while (gameEngine.isRunning())
	{
		gameEngine.update();

		gameEngine.render();
	}
}

