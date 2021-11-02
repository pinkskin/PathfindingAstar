#include "PathfindingAlgo.h"

//CONSTRUCTOR
PathfindingAlgo::PathfindingAlgo()
	:
	startNode(NULL), endNode(NULL)
{

}

/////////////////////// PUBLIC //////////////////////////////////////

std::vector<TileNode*>* PathfindingAlgo::getPath(TileNode& startNode, TileNode& endNode, Tilemap& tilemap)
{
	//init startNode local to 0
	startNode.local = 0.f;

	//set initial currentNode to startNode
	TileNode* currentNode = &startNode;

	//TODO:Make heuristic arg short. currently does nodeCenterPosCoords calculation on the spot
	//set startNode global to its heuristic
	tilemap.setTileNodeHeuristic(sf::Vector2f(startNode.screenPosUpperLeftVert.x + tilemap.tilesetTexture->getSize().y / 2, 
		startNode.screenPosUpperLeftVert.y + tilemap.tilesetTexture->getSize().y / 2),
		sf::Vector2f(endNode.screenPosUpperLeftVert.x + tilemap.tilesetTexture->getSize().y / 2,
			endNode.screenPosUpperLeftVert.y + tilemap.tilesetTexture->getSize().y / 2),
		*currentNode);
	currentNode->global = currentNode->heuristic;
	//std::cout << "startNode heuristic: " << currentNode->heuristic << std::endl;						//CHECKING startNode heuristic

	this->operationVector.push_back(currentNode); 

	//std::cout << "check adj nodes size: " << currentNode->adjacentNodes.size() << "\n";					//CHECKING adjnode size 
	//std::cout << "startNode address in pathfinding algo: " << &startNode << std::endl;					//CHECKING startNode address as an arg
	//std::cout << "endNode address in pathfinding algo: " << &endNode << std::endl;						//CHECKING endNode address as an arg
	//std::cout << "startNode adjNode[1] address: " << &startNode.adjacentNodes[1] << std::endl;			//CHECKING startNode adjNode[1] address
	
	//Main loop for pathfinding
	while (currentNode != &endNode)
	//for (int x = 0; x<5; ++x)
	{
		//for each of the adjacent nodes of the current...
		for (auto& x : currentNode->adjacentNodes)
		{
			//If current node local + current adj CTMI is < current adj local,
			//update that adj node
			float currentCTMI = currentNode->local + x->costToMoveInto;
			if (currentCTMI < x->local)
			{
				//below is what happens to a checked node
				x->local = currentCTMI;

				//TODO:Make heuristic arg short. currently does nodeCenterPosCoords calculation on the spot
				//set currnode heuristic 
				tilemap.setTileNodeHeuristic(sf::Vector2f(x->screenPosUpperLeftVert.x + tilemap.tilesetTexture->getSize().y / 2,
					x->screenPosUpperLeftVert.y + tilemap.tilesetTexture->getSize().y / 2),
					sf::Vector2f(endNode.screenPosUpperLeftVert.x + tilemap.tilesetTexture->getSize().y / 2,
						endNode.screenPosUpperLeftVert.y + tilemap.tilesetTexture->getSize().y / 2),
					*x);

				x->global = x->local + x->heuristic;
				x->parent = currentNode;
				this->operationVector.push_back(x);
				
				tilemap.changeTileNodeTexture(x->screenPosUpperLeftVert, 4);

				//std::cout << "check adj nodes size: " << this->operationVector.size() << "\n";			//CHECKING this->operationVector.size()
				//std::cout << "check adj nodes address: " << x << "\n";									//CHECKING adjnodes address
				//std::cout << "current adj currentCTMI: " << currentCTMI << "\n";						//CHECKING currentCTMI
				//std::cout << "current adj heuristic: " << x->heuristic << "\n";							//CHECKING current adj heuristic
				//std::cout << "current adj global: " << x->global << "\n";								//CHECKING current adj global
			}
		}
		//std::cout << this->operationVector.size() << "\n";
		//std::cout << "opvector front address: " << this->operationVector.front() << "\n";				//CHECKING opvector front address
		this->operationVector.pop_front();												//ERROR here
		//std::cout << "after popping opvector size: " << this->operationVector.size() << "\n";			//CHECKING opvector size after popping
		this->operationVector.sort([](TileNode* const& tN1, TileNode* const& tN2) //used lambda
			{
				return tN1->global < tN2->global;
			}); 
		currentNode = this->operationVector.front();
		//std::cout << "currentNode address: " << currentNode << "\n";									//CHECKING currentNode address 
	}

	//Generating optimal path from endNode to startNode
	currentNode = &endNode;
	while (currentNode != &startNode && endNode.parent != nullptr)
	{
		this->optimalPath.push_back(currentNode->parent);
		currentNode = currentNode->parent;
	}

	return &this->optimalPath;
}

void PathfindingAlgo::resetProperties(Tilemap& tilemap)
{
	//Resetting properties
	for (int i = 0; i < tilemap.tilemapHeight; ++i)
	{
		for (int j = 0; j < tilemap.tilemapWidth; ++j)
		{
			//Reset nodes' local and global variables
			tilemap.tileNodeVector[i * tilemap.tilemapWidth + j].local = std::numeric_limits<float>::infinity();
			tilemap.tileNodeVector[i * tilemap.tilemapWidth + j].global = std::numeric_limits<float>::infinity();

			//Reset textures to original
			sf::Vertex* quad = &tilemap.verticesVertArr[(i * tilemap.tilemapWidth + j) * 4];
			char currentTileElem = tilemap.tileBlueprint[i * tilemap.tilemapWidth + j];
			int intCurrentTileElem = 0;
			if (currentTileElem == '0')
				intCurrentTileElem = 0;
			if (currentTileElem == '1')
				intCurrentTileElem = 1;
			int tu = intCurrentTileElem % (tilemap.tilesetTexture->getSize().x / 32);				//static value of 32
			int tv = intCurrentTileElem / (tilemap.tilesetTexture->getSize().x / 32);				//static value of 32
			quad[0].texCoords = sf::Vector2f(tu * 32, tv * 32);
			quad[1].texCoords = sf::Vector2f((tu + 1) * 32, tv * 32);
			quad[2].texCoords = sf::Vector2f((tu + 1) * 32, (tv + 1) * 32);
			quad[3].texCoords = sf::Vector2f(tu * 32, (tv + 1) * 32);
		}
	}
	//Reset containers
	this->operationVector.clear();
	this->optimalPath.clear();
}
