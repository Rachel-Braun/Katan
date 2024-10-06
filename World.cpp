#include <iostream>
#include <sstream>
#include <memory>
#include <fstream>
#include <iostream>
#include "Input.h"
#include "json.hpp"
#include "useJSON.h"
#include "World.h"

useJSON World::jsonData;

World::World(vector<vector<string>> dataWorld)
{
	//Tiles
	for (int i = 0; i < dataWorld.size(); i++)
	{
		vector<Tile> vec;
		for (int j = 0; j < dataWorld[i].size(); j++)
		{
			vec.push_back(Tile(stoi(dataWorld[i][j])));
		}
		board.push_back(vec);
	}

	//Cells
	cells = vector<vector<Cell>>(dataWorld.size()*Tile::size[0], vector<Cell>());
	for (vector<Cell>& v : cells)
	{
		v.resize(dataWorld[0].size() * Tile::size[1]);
	}
}

string World::getCategoryByLocation(int x, int y)
{
	if(!empty(x, y))
		return cells[y][x].objectPtr->category;
	if (!itemEmpty(x, y))
		return cells[y][x].itemPtr->category;
	return board[(y / Tile::size[0])][(x / Tile::size[1])].category;
}

bool World::empty(int x, int y)
{
	return cells[y][x].objectPtr == nullptr;
}
bool World::itemEmpty(int x, int y)
{
	return cells[y][x].itemPtr == nullptr;
}

void World::initialResource(int x, int y, int amount, string resource)
{
	if (!empty(x, y))  //city or village
		cells[y][x].objectPtr->addResource(resource, amount);
	else if(!itemEmpty(x ,y))
		cells[y][x].itemPtr->addResource(resource, amount);
	else
		board[y / Tile::size[0]][x / Tile::size[1]].amount += amount;

	amountsOfResource[resource] += amount;
}

void World::initialResources(int x, int y, vector<int> resources)
{
	vector<string>resourcesName = jsonData.getResourcesType();
	Object*& objectToInitial = !empty(x, y) ? cells[y][x].objectPtr : cells[y][x].itemPtr;
	for(int i=0; i<resourcesName.size()-1; i++)
		objectToInitial->amountsOfResource[resourcesName[i]] += resources[i];
}

string World::getIsComplete(int x, int y)
{
	return !empty(x, y) && cells[y][x].objectPtr->ConstructionTime
		>=jsonData.getTimeOf(cells[y][x].objectPtr->category)?"True":"False";
}

void World::resetResources(int x, int y)
{
	for (auto it : amountsOfResource)
		it.second = 0;
}

static bool availableResources(string category, Object* object)
{
	vector<int>costs = World::jsonData.getCostsOf(category);
	vector<string>resources = World::jsonData.getResourcesType();
	for (int i = 0; i < costs.size(); i++)
		if (object->amountsOfResource[resources[i]] < costs[i])
			return false;
	return true;
}

void World::manufacture(string category, int x, int y, bool isStart)
{
	vector<int>size = jsonData.getSize(category);
	//בדיקת תקינות
	//.........
	if (!isStart)
	{
		if (!availableResources(category, cells[y][x].objectPtr) || empty(x, y))
			return;
		for (int i = 0; i < size[0]; i++)
			for (int j = 0; j < size[1]; j++)
			{
				if (cells[y + i][x + j].itemPtr != nullptr && cells[y][x].itemPtr->category == "People")
					return;
			}

		if (cells[y][x].objectPtr->category == "Road")
		{
			for (int i = 0; i < size[0]; i++)
				for (int j = 0; j < size[1]; j++)
				{
					if (cells[y + i][x + j].objectPtr->category != "Road")
						return;
				}
		}
	}

	Object* object = new Object(category);
	for (int i = 0; i < size[0]; i++)
		for (int j = 0; j < size[1]; j++)
			cells[y+i][x+j].itemPtr = object;

	if(!empty(x, y))
		cells[y][x].objectPtr->amountsOfResource[category]++;
	itemCount[category]++;
}

void World::build(string category, int x, int y, bool isStart, int time)
{
	vector<int>size = jsonData.getSize(category);

	if (!isStart && !isPlaceMatchesConstraints(x, y, size[0], size[1]))
		return;

	Object* object = new Object(category);
	
	for (int i = y; i - y < size[0]; i++)
		for (int j = x; j - x < size[1]; j++)
		{
			cells[i][j].objectPtr = (object);
		}

	object->ConstructionTime += time;
	itemCount[category]++;
}

bool World::isPlaceMatchesConstraints(int x, int y, int size1, int size2)
{
	if(y>0) //up
	for (int i = x - 1<0?x:x-1; i <= x + size1 && i<cells[0].size(); i++)
	{
		if (cells[y - 1][i].objectPtr!=nullptr && cells[y - 1][i].objectPtr->category == "Road")
			return true;
	}
	if (y+size1 < cells.size()-1) //bottom
		for (int i = x - 1 < 0 ? x : x - 1; i <= x + size1 && i < cells[0].size(); i++)
		{
			if (cells[y + size2][i].objectPtr != nullptr && cells[y + size2][i].objectPtr->category == "Road")
				return true;
		}

	if (x > 0) //left
		for (int i = y - 1 < 0 ? y : y - 1; i <= y + size2 && i < cells.size(); i++)
		{
			if (cells[i][x - 1].objectPtr != nullptr && cells[i][x-1].objectPtr->category == "Road")
				return true;
		}
	if (x+size1 < cells[0].size() - 1)  //right
		for (int i = y - 1 < 0 ? y : y - 1; i <= y + size2 && i < cells.size(); i++)
		{
			if (cells[i][x+size1].objectPtr != nullptr && cells[i][x + size1].objectPtr->category == "Road")
				return true;
		}
	return false;
}

string World::selectedResource()
{
	string result = "SelectedResource ";
	vector<string>resources = jsonData.getResourcesType();

	if (!empty(selectedX, selectedY))
	{
		for (int i = 0; i < resources.size() - 1; i++)
			result += to_string(cells[selectedY][selectedX].objectPtr->amountsOfResource[resources[i]]) + " ";
	}
	else
	{
		string currentResource = jsonData.getResourceName(board[selectedY / Tile::size[0]][selectedX / Tile::size[1]].categoryId);
		
		for (int i = 0; i < resources.size() - 1; i++)
		{
			if (currentResource == resources[i])
				result += to_string(board[selectedY / Tile::size[0]][selectedX / Tile::size[1]].amount) + " ";
			else
				result += "0 ";
		}
	}
	return result;
}

void World::growResources(int time)
{
	map<string, int>data = jsonData.getResourcesGrowInRain();
	for (int i = 0; i < board.size(); i++)
		for (int j = 0; j < board[i].size(); j++)
		{
			if (data.find(board[i][j].resource) != data.end()) //זה משאב שצומח בגשם
				board[i][j].amount += time / data[board[i][j].resource];
		}
}

void World::takeResources(int x, int y)
{
	string resource; 
	auto& resourcesOfSource = cells[y][x].objectPtr->amountsOfResource;
	auto& resourcesOfDestination = cells[selectedY][selectedX].itemPtr->amountsOfResource;
	int maxCapacity = jsonData.getMaxCapacity(cells[selectedY][selectedX].itemPtr->category, resource);
	int capacityBefore = resourcesOfDestination[resource];

	for (auto it = resourcesOfSource.begin(); it != resourcesOfSource.end(); it++)
		if (it->second != 0)
		{
			resource = it->first;
			break;
		}
	
	if (resourcesOfDestination[resource] == maxCapacity)
		return;

	resourcesOfDestination[resource] = min(maxCapacity,  capacityBefore + resourcesOfSource[resource]);
	resourcesOfSource[resource] -= (maxCapacity - capacityBefore);
}


