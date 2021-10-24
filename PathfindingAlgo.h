#pragma once

#include <list>
#include "TileMap.h"

class PathfindingAlgo
{
public:
	PathfindingAlgo();

	//Public VARIABLES
	TileNode* startNode; //unused yet
	TileNode* endNode;	//unused yet

	//Public FUNCTIONS
	std::vector<TileNode*>* getPath(TileNode& startNode, TileNode& endNode);

private:
	//Private VARIABLES
	std::vector<TileNode*> optimalPath;
	std::list<TileNode*> operationVector;

};

