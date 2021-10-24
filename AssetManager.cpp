#include "AssetManager.h"

AssetManager* AssetManager::sInstance = nullptr; // try commenting this

//CONSTRUCTOR
AssetManager::AssetManager()
{
	//Only one AssetManager to exist
	//otherwise throw exception
	assert(sInstance == nullptr);
	this->sInstance = this;
}

sf::Texture& AssetManager::getTexture(std::string const& filename)
{
	auto& textureMap = sInstance->m_Textures; //or just use std::map<std::string, sf::Texture>    tried using auto haha

	//see if texture is already loaded
	auto pairFound = textureMap.find(filename);
	//if yes, return the texture 
	if (pairFound != textureMap.end())
	{
		return pairFound->second;
	}
	else //else load texture and return it 
	{
		//create an element in the texture map
		auto& texture = textureMap[filename];
		//put the .second value of the map
		assert(texture.loadFromFile(filename));
		return texture;
	}
}


