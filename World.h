#pragma once
#include <vector>
#include <string>
#include "Tile.h"
#include "Cell.h"
using namespace std;
class World
{
	friend class Functions;
	public:
		vector<vector<Tile>> board;
		vector<vector<Cell>> cells;
		int selectedX = 0;
		int selectedY = 0;
		unordered_map<string, int>itemCount;
		unordered_map<string, int>amountsOfResource;
		static useJSON jsonData;

		World(vector<vector<string>> dataWorld);

		~World(){};
		
		string getCategoryByLocation(int x, int y);
		
		void initialResource(int x, int y, int amount, string resource);
		
		string getIsComplete(int x, int y);
		
		bool empty(int x, int y);
		
		bool itemEmpty(int x, int y);
		
		void resetResources(int x, int y);
		
		void manufacture(string category, int x, int y, bool isStart);
		
		void build(string category, int x, int y, bool isStart, int time);
		
		bool isPlaceMatchesConstraints(int x, int y, int size1, int size2);
		
		string selectedResource();
		
		void growResources(int time);
		
		void initialResources(int x, int y, vector<int> resources);
		
		void takeResources(int x, int y);
};

