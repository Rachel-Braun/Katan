#include "useJSON.h"
using namespace nlohmann;

json useJSON::config = NULL;

useJSON::useJSON()
{
    ifstream ifs("configur.json");
    if (!ifs.is_open()) 
        throw runtime_error("Failed to open JSON file: JSON.json");
    
    ifs >> config;
    ifs.close();
}


string useJSON::getTileName(int i)
{
    return config["TilesName"][i - 1];
}

string useJSON::getResourceName(int i)
{
    return config["ResourceTypes"][i - 3];
}

int useJSON::getMaxCapacity(string category, string resource)
{
    vector<string>resources = getResourcesType();
    int result=0;
    for (int i = 0; i < resources.size(); i++)
        if (resources[i] == resource)
            result = i;

    return config["Capacities"][category][result];
}

map<string, int> useJSON::getResourcesGrowInRain()
{
    map<string, int>result;
    for (json::iterator it = config["Rains"].begin(); it != config["Rains"].end(); ++it)
        result[it.key()] = it.value();
    return result;
}

int useJSON::getTimeOf(string category)
{
    return config["Times"][category];
}

vector<int> useJSON::getCostsOf(string category)
{
    vector<int>result;
    for (int i = 0; i < config["Costs"][category].size(); i++)
        result.emplace_back(config["Costs"][category][i]);
    return result;
}

float useJSON::getSpeedOf(string category)
{
    return config["Speeds"][category];
}

vector<int> useJSON::getSize(string item)
{
    return config["Sizes"][item];
}

vector<string> useJSON::getResourcesType()
{
    vector<string>result;
    for (int i = 0; i < config["ResourceTypes"].size(); i++)
        result.push_back(config["ResourceTypes"][i]);
    return result;
}

