#pragma once
// Include for using shared_ptr
#include "Object.h"
#include <memory>
using namespace std;

class Cell
{
public:
    Object* objectPtr = nullptr;
    Object* itemPtr = nullptr;
};

