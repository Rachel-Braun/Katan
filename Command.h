#pragma once
#include <string>
#include <vector>
using namespace std;

class Command
{
public:
    Command(const std::string& _name) : name(_name) { }

    std::string name;
    std::vector<std::string> arguments;
    std::vector< std::vector<std::string> > data;

    static const std::string WORLD;
    static const std::string START;
    static const std::string INPUT;
    static const std::string ASSERTS;

    bool operator==(const Command& other) const {
        return name == other.name; // Customize this comparison based on your requirements
    }

    static const std::string WAIT;

    static const std::string SELECT;
    static const std::string MOVE;
    static const std::string WORK;
    static const std::string DEPOSIT;
    static const std::string TAKE_RESOURCES;
    static const std::string BUILD;
    static const std::string MANUFACTURE;
    static const std::string PEOPLE;
    static const std::string RESOURCE;
    static const std::string RESOURCES;
    static const std::string MAKE_EMPTY;
    static const std::string RAIN;
    static const std::string ROBBER;
    static const std::string MAKE_ROBBER;
};

