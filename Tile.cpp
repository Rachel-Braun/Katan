#include "Tile.h"

useJSON Tile::config;
vector<int> Tile::size = Tile::config.getSize("Tile");

Tile::Tile(int id)
{
	this->categoryId = id;
	this->category =Tile::config.getTileName(id);
	resource = id >= 3 ? Tile::config.getResourceName(id) : "";
}

