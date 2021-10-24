#include "GameEngine.h"

//CONSTRUCTOR
GameEngine::GameEngine()
{
	this->initVariables();
	this->initWindow();
	this->initEntities();
}

void GameEngine::pollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			this->handleKeypresses(this->event);
			break;
		case sf::Event::MouseButtonPressed:
			this->handleMousePresses(this->event);
		}
	}
}

//Handles events from pollEvent (keyboard)
void GameEngine::handleKeypresses(sf::Event event)
{
	if (this->event.key.code == sf::Keyboard::Escape)
		this->window->close();
}

//Handles events from pollEvent (mouse) //TODO CURRENT
void GameEngine::handleMousePresses(sf::Event evenjt)
{

	//set start node
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		this->tilemap.changeTileNodeTexture(this->mousePosView, event);

		//set startnode for algo
		this->aStarAlgo.startNode = this->tilemap.getNodeAtMousePos(this->mousePosView);

		//std::cout << tilemap.getQuadCenterPos(this->mousePosView).x << ","
		//	tilemap.getQuadCenterPos(this->mousePosView).y << std::endl;
	}
	//set end node
	if (event.mouseButton.button == sf::Mouse::Right)
	{
		this->tilemap.changeTileNodeTexture(this->mousePosView, event);

		//set endNode for algo
		this->aStarAlgo.endNode = this->tilemap.getNodeAtMousePos(this->mousePosView);

		std::cout << "startNode address in astaralgo: " << this->aStarAlgo.startNode << std::endl;			//CHECKING startNode address in astar algo
		std::cout << "endNode address in astaralgo: " << this->aStarAlgo.endNode << std::endl;				//CHECKING endNode address in astar algo

		//conclude the algo
		std::vector<TileNode*>* optimalPath;
		optimalPath = this->aStarAlgo.getPath(*this->aStarAlgo.startNode, *this->aStarAlgo.endNode);
		//std::cout << optimalPath->size() << std::endl;
	}
}

void GameEngine::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

//Main update function
void GameEngine::update()
{
	this->pollEvents();
	updateMousePositions();
	//std::cout << this->mousePosView.x << "," << this->mousePosView.y << std::endl;			//CHECKING
}

void GameEngine::render()
{
	this->window->clear(sf::Color::Black);
	
	//draw here:
	this->window->draw(this->tilemap);

	this->window->display();
}

bool GameEngine::isRunning()
{
	return this->window->isOpen();
}

void GameEngine::initVariables()
{
	this->window = nullptr;
	this->windowHeight = 800;
	this->windowWidth = 800;
}

void GameEngine::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(
		this->windowWidth, this->windowHeight), "A* Algorithm");
}

void GameEngine::initEntities()
{
	std::string mapBluePrint = "";
	mapBluePrint += "1000000000000001";
	mapBluePrint += "0000001100111100";
	mapBluePrint += "0000001100100100";
	mapBluePrint += "0000001100111100";
	mapBluePrint += "0000001100100100";
	mapBluePrint += "0001111100111100";
	mapBluePrint += "0000000000000000";
	mapBluePrint += "1000000000000001";

	this->tilemap.load("assets/tiles/sfml_tileset.png", sf::Vector2u(32, 32), mapBluePrint, 16, 8);
}
