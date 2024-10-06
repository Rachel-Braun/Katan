#pragma once
#include <string>
#include "useJSON.h"
#include "Object.h"
using namespace std;
class Tile
{
	friend class World;
public:
	static useJSON config;
	int categoryId;
	string category;
	string resource;
	int amount = 0;
	int peopleCount = 0;
	static vector<int> size;

	Tile(int id);
};

