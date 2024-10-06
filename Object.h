#pragma once
#include <string>
#include <unordered_map>
using namespace std;

class Object
{
	public:
		unordered_map<string, int>amountsOfResource;
		string category;
		int ConstructionTime = 0;
		Object(string name) :category(name) {};
		int addResource(string resource, int amount);
};

