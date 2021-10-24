#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>

class TileNode
{
public:
	//CONSTRUCTORS
	TileNode();
	//TileNode(int width, int height, int costToMoveInto);

	//Public VARIABLES
	sf::Vector2f screenPosUpperLeftVert;
	std::string type;
	float costToMoveInto;
	float heuristic;
	float local;
	float global;
	TileNode* parent;
	std::vector<TileNode*> adjacentNodes;

private:

};

