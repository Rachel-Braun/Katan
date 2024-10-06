#pragma once
#include <map>
#include <string>
#include <algorithm>
#include "World.h"
#include "Command.h"
#include "climits"
using namespace std;

class Functions
{
public:
    map<string, function<void(World&, Command)>> functionsMap;
    vector<Command>waits;
    bool isStart = true;
    Functions();
};