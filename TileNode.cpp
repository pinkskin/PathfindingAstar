#include "TileNode.h"

//CONSTRUCTORS
//Default constructor
TileNode::TileNode()
	:
	screenPosUpperLeftVert(), type("empty"), costToMoveInto(1.f),
	heuristic(0.f), local(std::numeric_limits<float>::infinity()),
	global(std::numeric_limits<float>::infinity()), 
	parent(nullptr), adjacentNodes()
{

}
