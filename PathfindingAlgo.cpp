#include "PathfindingAlgo.h"

//CONSTRUCTOR
PathfindingAlgo::PathfindingAlgo()
	:
	startNode(NULL), endNode(NULL)
{

}

/////////////////////// PUBLIC //////////////////////////////////////

std::vector<TileNode*>* PathfindingAlgo::getPath(TileNode& startNode, TileNode& endNode)
{
	//set node local to 0
	startNode.local = 0.f;
	//set initial currentNode to startNode
	TileNode* currentNode = &startNode;
	this->operationVector.push_back(currentNode); //std::cout << (operationVector.size()) << "\n";

	std::cout << "check adj nodes size: " << currentNode->adjacentNodes.size() << "\n";					//CHECKING adjnode size 
	std::cout << "startNode address in pathfinding algo: " << &startNode << std::endl;					//CHECKING startNode address as an arg
	std::cout << "endNode address in pathfinding algo: " << &endNode << std::endl;						//CHECKING endNode address as an arg
	std::cout << "startNode adjNode[1] address: " << &startNode.adjacentNodes[1] << std::endl;			//CHECKING startNode adjNode[1] address
	

	/////////////////////////////////TODO: HEURISTIC IS THE MAIN PROBLEM. CHECK setTileNodeHeuristic/////////////////////////////////


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
				x->global = x->local + x->heuristic;
				x->parent = currentNode;
				this->operationVector.push_back(x);


			

				std::cout << "check adj nodes size: " << this->operationVector.size() << "\n";			//CHECKING this->operationVector.size()
				std::cout << "check adj nodes address: " << x << "\n";									//CHECKING adjnodes address
				std::cout << "current adj currentCTMI: " << currentCTMI << "\n";						//CHECKING currentCTMI
				std::cout << "current adj heuristic: " << x->heuristic << "\n";							//CHECKING current adj heuristic
				std::cout << "current adj global: " << x->global << "\n";								//CHECKING current adj global
			}
		}
		//std::cout << this->operationVector.size() << "\n";
		std::cout << "opvector front address: " << this->operationVector.front() << "\n";				//CHECKING opvector front address
		this->operationVector.pop_front();												//ERROR here
		std::cout << "after popping opvector size: " << this->operationVector.size() << "\n";			//CHECKING opvector size after popping
		this->operationVector.sort([](TileNode* const& tN1, TileNode* const& tN2) //used lambda
			{
				return tN1->global < tN2->global;
			}); 
		currentNode = this->operationVector.front();
		std::cout << "currentNode address: " << currentNode << "\n";									//CHECKING currentNode address 
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
