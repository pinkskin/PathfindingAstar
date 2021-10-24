#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "AssetManager.h"
//#include "Tilemap.h"
#include "PathfindingAlgo.h"

class GameEngine
{
private:
	//Private VARIABLES
	sf::RenderWindow* window;
	int windowHeight;
	int windowWidth;
	sf::Event event;
	//Mouse
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	//Logic
	PathfindingAlgo aStarAlgo;

	//Private FUNCTIONS
	void initVariables();
	void initWindow();
	void initEntities();

public:
	//CONSTRUCTOR
	GameEngine();

	//public VARIABLES
	Tilemap tilemap;
	sf::Clock clock;

	//Public FUNCTIONS
	void pollEvents();
	void handleKeypresses(sf::Event event);
	void handleMousePresses(sf::Event event);
	void updateMousePositions();
	void update(); //main update function
	void render(); //main render function

	//Public helper FUNCTIONS
	bool isRunning();
};