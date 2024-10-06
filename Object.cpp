#include "Object.h"
#include "useJSON.h"

int Object::addResource(string resource, int amount)
{
    useJSON jsonData;
    int maxCapacity = jsonData.getMaxCapacity(category, resource);
    if (amount + amountsOfResource[resource] <= maxCapacity)
    {
        amountsOfResource[resource] += amount;
        return 0;
    }
    int remainder = maxCapacity - amountsOfResource[resource];
    amountsOfResource[resource] = maxCapacity;
    return amount - remainder;
}
