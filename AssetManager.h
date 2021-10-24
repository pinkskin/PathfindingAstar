#pragma once

#include <SFML/Graphics.hpp>
#include <assert.h>
#include <map>

/*
	Class for managing assets
*/

class AssetManager
{
public:
	//CONSTRUCTOR
	AssetManager();

	//Functions
	static sf::Texture& getTexture(std::string const& filename);
private:
	//Private VARIABLES
	std::map<std::string, sf::Texture> m_Textures;
	//AssetManager is a singleton. Only 1 instance allowed
	//sInstance holds a pointer to the single instance
	static AssetManager* sInstance;
};

