#pragma once
#include <fstream>
#include <string>
#include "json.hpp"
#include <memory>
using namespace nlohmann;
using namespace std;
using json = nlohmann::json;
class useJSON
{
public:
	static json config;
	useJSON();

	string getTileName(int i);
	vector<int> getSize(string item);
	vector<string>getResourcesType();
	string getResourceName(int id);
	int getMaxCapacity(string category, string resource);
	map<string, int>getResourcesGrowInRain();
	int getTimeOf(string category);
	vector<int> getCostsOf(string category);
	float getSpeedOf(string category);
};

