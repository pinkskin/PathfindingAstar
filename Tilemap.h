#pragma once

#include <iostream>
#include "TileNode.h"
#include "AssetManager.h"
//#include "PathfindingAlgo.h"

/*
	Class for making a tilemap using vertex arrays
*/

class Tilemap : public sf::Transformable, public sf:: Drawable
{
public:
	Tilemap();

	//public FUNCTIONS ////////////////////////
	void setTileNode(TileNode& tileNode); // UNUSED
	void setTileNodesAdj();
	void setTileNodeHeuristic(sf::Vector2f const& quadCenterStart, 
		sf::Vector2f const& quadCenterEnd, TileNode& currentTileNode); //TODO: make the param take quadUpperLeftVertCoord then calculate center inside
	//sf::vector2f getSize(); //TODO
	TileNode* getNodeAtMousePos(sf::Vector2f const& mousePosView);
	char getElemAtMousePos(sf::Vector2f const& mousePosView);
	//sf::Vector2f getTileMapCenterPos(); //TODO
	bool load(std::string const& tileTexture, sf::Vector2u tileTextureSize, 
		std::string tileBlueprint, int tilesWidth, int tilesHeight); //try making tileBlueprint const&
	void changeTileNodeTexture(sf::Vector2f const& mousePosView, sf::Event event); //TODO: change a single tilenode's texture
	void changeTileNodeTexture(sf::Vector2f const& tileNodeUpLfVertScreenPos, int texPos); //TODO: change a single tilenode's texture (static)

	//helper FUNCTIONS 
	//sf::Vector2f getQuadCenterPos(sf::Vector2f const& quadPos0, sf::Vector2f const& quadPos3); //Calculate a quad's center position. Returns Vector2f
	sf::Vector2f getQuadCenterPos(sf::Vector2f const& mousePosViewCoords); //for mouseclick on a tileNode - Calculate a quad's center position. Returns Vector2f

	//Public VARIABLES ////////////////////
	sf::Texture* tilesetTexture; //used a pointer here because must not make a copy of texture from asset manager. see https://en.sfml-dev.org/forums/index.php?topic=27616.0
	std::vector<TileNode> tileNodeVector;
	sf::VertexArray verticesVertArr;
	std::string tileBlueprint;
	int tilemapWidth;
	int tilemapHeight;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Private VARIABLES //////////////////
	
	//PathfindingAlgo pathFinder; //TODO apply a* in tilemap
};

