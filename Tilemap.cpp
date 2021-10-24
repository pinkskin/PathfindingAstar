#include "Tilemap.h"



//CONSTRUCTORS
//Default Constructor
Tilemap::Tilemap()
	:
	verticesVertArr(), tilesetTexture(nullptr), tileBlueprint(""),
	tilemapWidth(0), tilemapHeight(0), tileNodeVector()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Private FUNCTIONS

//Sfml drawable virtual function definition
void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//apply the transform 
	states.transform *= getTransform();

	//apply the tileset texture
	states.texture = *&this->tilesetTexture;

	//draw the vertex array
	target.draw(this->verticesVertArr, states);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Public FUNCTIONS

//set the properties of a single tilenode except adj nodes
//UNUSED
void Tilemap::setTileNode(TileNode& tileNode)
{
	/*
		set TileNode variables
		- string type;
		- int costToMoveInto;
		- int heuristic;
		- int local
		- int global
		- TileNode* parent;
	*/
	tileNode.type = "foo";
	tileNode.costToMoveInto = 0;
	tileNode.local = std::numeric_limits<float>::infinity();
	tileNode.global = std::numeric_limits<float>::infinity();
	tileNode.heuristic = 0;
	tileNode.parent = nullptr;
}

//Create the tilemap entity
bool Tilemap::load(std::string const& tileTexture, sf::Vector2u tileTextureSize,
	std::string tileBlueprint, int tilesWidth, int tilesHeight)
{
	this->tileBlueprint = tileBlueprint;							//make own copy of tileblueprint
	this->tilemapWidth = tilesWidth;								//set member tilesMapWidth 
	this->tilemapHeight = tilesHeight;								//set member tilesMapHeight
	this->tilesetTexture = &AssetManager::getTexture(tileTexture);	//texture copy
	this->verticesVertArr.setPrimitiveType(sf::Quads);				//set vertarr primitive type (quads)
	this->verticesVertArr.resize(tilesWidth * tilesHeight * 4);		//resize according to number of verts(quads)
	this->tileNodeVector.resize(tileBlueprint.length());			//populate tileNodeVector to the same size as tileBlueprint
	this->setTileNodesAdj();										//set adjacent nodes

	for (int i = 0; i < tilesHeight; ++i)
	{
		for (int j = 0; j < tilesWidth; ++j)
		{
			//current TileNode
			TileNode& currTileNode = this->tileNodeVector[i * this->tilemapWidth + j];
			//Set currentTileNode's screenPosUpperLeftVert
			currTileNode.screenPosUpperLeftVert.x = j * this->tilesetTexture->getSize().y;
			currTileNode.screenPosUpperLeftVert.y = i * this->tilesetTexture->getSize().y;
			//current tile element (char)
			char currentTileElem = this->tileBlueprint[i * this->tilemapWidth + j];
			int intCurrentTileElem = 0;
			if (currentTileElem == '0')
				intCurrentTileElem = 0;
			if (currentTileElem == '1')
				intCurrentTileElem = 1;
			//std::cout << currentTileElem << " ";												//CHECKING tile blueprint

			//find position in tileset texture ----- TODO: change this for your texture needs
			int tu = intCurrentTileElem % (this->tilesetTexture->getSize().x / tileTextureSize.x);
			int tv = intCurrentTileElem / (this->tilesetTexture->getSize().x / tileTextureSize.x);

			//get pointer to current tile's quad upper left vector
			sf::Vertex* quad = &this->verticesVertArr[(i * tilesWidth + j) * 4];
			//std::cout << (i * tilesWidth + j) * 4 << " ";						     			//CHECKING verticesVertArr indices

			//define 4 corners of the quad 
			quad[0].position = sf::Vector2f(j * tileTextureSize.x, i * tileTextureSize.y);
			quad[1].position = sf::Vector2f((j + 1) * tileTextureSize.x, i * tileTextureSize.y);	
			quad[2].position = sf::Vector2f((j + 1) * tileTextureSize.x, (i + 1) * tileTextureSize.y);
			quad[3].position = sf::Vector2f(j * tileTextureSize.x, (i + 1) * tileTextureSize.y);

			//Calculate center position of each quad 
			sf::Vector2f currQuadCenter = this->getQuadCenterPos(sf::Vector2f(quad[0].position.x, quad[0].position.y), 
				sf::Vector2f(quad[3].position.x, quad[3].position.y));





				
			//TODO:Separate setting of node heuristic from here to enable updating. Or just figure out how to update heuristic
			//clue: use getQuadCenterPos(sf::Vector2f const& mousePosViewCoords)

			//std::cout << currQuadCenter.x << "," << currQuadCenter.y << " ";					//CHECKING quadCenter output
			this->setTileNodeHeuristic(currQuadCenter, sf::Vector2f(16,16), currTileNode); //2nd arg is currently static val. 
			//std::cout << static_cast<int>(currTileNode.heuristic) << " ";						//CHECKING current TileNode's heuristic
			
			
			
			
			
			
			//std::cout << currTileNode.adjacentNodes.size() << " ";								//CHECKING adj size

			//define texture coords
			quad[0].texCoords = sf::Vector2f(tu * tileTextureSize.x, tv * tileTextureSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileTextureSize.x, tv * tileTextureSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileTextureSize.x, (tv + 1) * tileTextureSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileTextureSize.x, (tv + 1) * tileTextureSize.y);
		}
		std::cout << std::endl;																	//CHECKING
	}
	return true;
}

//TODO: fix texture usage here
//Change a single tilenode's texture //TODO: CURRENT
void Tilemap::changeTileNodeTexture(sf::Vector2f const& mousePosView, sf::Event event)
{
	//xPos and yPos is limiting the coords to be multiples of texture size
	int xPos = mousePosView.x - (static_cast<int>(mousePosView.x) % this->tilesetTexture->getSize().y);
	int yPos = mousePosView.y - (static_cast<int>(mousePosView.y) % this->tilesetTexture->getSize().y);
	//adjust xPos and yPos to be just [0, 1, 2, 3, 4...]
	int xPosAdjusted = xPos / this->tilesetTexture->getSize().y;
	int yPosAdjusted = yPos / this->tilesetTexture->getSize().y;
	//std::cout << xPosAdjusted << "," << yPosAdjusted << std::endl;												//CHECKING

	//get pointer to current tile's quad upper left vector
	sf::Vertex* quad = &this->verticesVertArr[(yPosAdjusted * 
		this->tilemapWidth + xPosAdjusted) * 4];

	//currently the texture is constant at pos 4
	int texturePos = 4;
	if (event.key.code == sf::Mouse::Right)
	{
		texturePos = 2;
	}
	quad[0].texCoords = sf::Vector2f(texturePos * 32, 0 * 32);
	quad[1].texCoords = sf::Vector2f((texturePos + 1) * 32, 0 * 32);
	quad[2].texCoords = sf::Vector2f((texturePos + 1) * 32, (0 + 1) * 32);
	quad[3].texCoords = sf::Vector2f(texturePos * 32, (0 + 1) * 32);
}
//TODO: FIX size. texture size here is still static
void Tilemap::changeTileNodeTexture(sf::Vector2f const& tileNodeUpLfVertScreenPos)
{
	//adjust xPos and yPos to be just [0, 1, 2, 3, 4...]
	int xPosAdjusted = tileNodeUpLfVertScreenPos.x / 32;
	int yPosAdjusted = tileNodeUpLfVertScreenPos.y / 32;

	std::cout << xPosAdjusted << "," << yPosAdjusted;

	sf::Vertex* quad = &verticesVertArr[(yPosAdjusted *
		tilemapWidth + xPosAdjusted) * 4];

	//currently the texture is constant at pos 4
	int texturePos = 4;
	quad[0].texCoords = sf::Vector2f(texturePos * 32, 0 * 32);
	quad[1].texCoords = sf::Vector2f((texturePos + 1) * 32, 0 * 32);
	quad[2].texCoords = sf::Vector2f((texturePos + 1) * 32, (0 + 1) * 32);
	quad[3].texCoords = sf::Vector2f(texturePos * 32, (0 + 1) * 32);
}

//Return the node at the mouse position
TileNode* Tilemap::getNodeAtMousePos(sf::Vector2f const& mousePosView)
{
	//xPos and yPos is limiting the coords to be multiples of texture size (points to upper left quad vertex)
	int xPos = mousePosView.x - (static_cast<int>(mousePosView.x) % this->tilesetTexture->getSize().y);
	int yPos = mousePosView.y - (static_cast<int>(mousePosView.y) % this->tilesetTexture->getSize().y);
	//adjust xPos and yPos to be just [0, 1, 2, 3, 4...]
	int xPosAdjusted = xPos / this->tilesetTexture->getSize().y;
	int yPosAdjusted = yPos / this->tilesetTexture->getSize().y;

	//std::cout << yPosAdjusted * this->tilemapWidth + xPosAdjusted << std::endl;							//CHECKING tileNodeVector index

	return &this->tileNodeVector[yPosAdjusted * this->tilemapWidth + xPosAdjusted];
}


//Calculate a quad's center position. Returns Vector2f
sf::Vector2f Tilemap::getQuadCenterPos(sf::Vector2f const& quadPos0, sf::Vector2f const& quadPos3)
{
	sf::Vector2f quadCenterPos = quadPos0; //initialize to the current quad's upper left vector pos
	int squareHalfLength = (quadPos3.y - quadPos0.y) / 2;
	quadCenterPos.x += squareHalfLength;
	quadCenterPos.y += squareHalfLength;

	return quadCenterPos;
}
//for mouseclick on a tileNode - Calculate a quad's center position. Returns Vector2f
sf::Vector2f Tilemap::getQuadCenterPos(sf::Vector2f const& mousePosViewCoords)
{
	//xPos and yPos is limiting the coords to be multiples of texture size (points to upper left quad vertex)
	int xPos = mousePosViewCoords.x - (static_cast<int>(mousePosViewCoords.x) % this->tilesetTexture->getSize().y);
	int yPos = mousePosViewCoords.y - (static_cast<int>(mousePosViewCoords.y) % this->tilesetTexture->getSize().y);
	xPos += this->tilesetTexture->getSize().y / 2;
	yPos += this->tilesetTexture->getSize().y / 2;

	return sf::Vector2f(xPos, yPos);
}


//sets the adjacent nodes of all tilenodes (manhattan distance[4 directions])
void Tilemap::setTileNodesAdj()
{
	/*
		Helper function to set TileNode adjacent nodes
		-TODO: take note of matrix corners and sides
	*/

	//Matrix Corners helper variables
	// 0 = uLeft
	int uRight = this->tilemapWidth - 1;
	int lLeft = this->tilemapWidth * (this->tilemapHeight - 1);
	int lRight = this->tilemapWidth * this->tilemapHeight - 1;

	for (int x = 0; x < this->tileNodeVector.size(); ++x)
	{
		//corners cases
		if (x == 0) //if upper left elem
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + this->tilemapWidth]);
		}
		else if (x == uRight) //if upper right elem
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + this->tilemapWidth]);
		}
		else if (x == lLeft) //if lower left elem
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - this->tilemapWidth]);
		}
		else if (x == lRight) //if lower right elem
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - this->tilemapWidth]);
		}
		//outer rows/cols cases
		else if (x > 0 && x < this->tilemapWidth - 1) //if upper row excluding the corners
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + this->tilemapWidth]);
		}
		else if (x != 0 && x != lLeft && x % this->tilemapWidth == 0) //if left column excluding the corners
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - this->tilemapWidth]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + this->tilemapWidth]);
		}
		else if (x != uRight && x != lRight && (x + 1) % this->tilemapWidth == 0) //if right column excluding the corners
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - this->tilemapWidth]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + this->tilemapWidth]);
		}
		else if (x > this->tilemapWidth * (this->tilemapHeight - 1) && x < this->tilemapWidth * this->tilemapHeight - 1) //if bottom row excluding the corners
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - this->tilemapWidth]);
		}
		else //else, x index is the inner nodes
		{
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + 1]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x + this->tilemapWidth]);
			this->tileNodeVector[x].adjacentNodes.push_back(&this->tileNodeVector[x - this->tilemapWidth]);
		}
	}
}

//Set the current TileNode's quad's heuristic by finding hypothenus
void Tilemap::setTileNodeHeuristic(sf::Vector2f const& startQuadCenter, 
	sf::Vector2f const& endQuadCenter, TileNode& currentTileNode)
{
	float a = endQuadCenter.x - startQuadCenter.x;
	float b = endQuadCenter.y - startQuadCenter.y;
	float c = 0;
	c = sqrt(a * a + b * b);

	currentTileNode.heuristic = c;
}
