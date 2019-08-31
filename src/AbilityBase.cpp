#include "AbilityBase.hpp"

AbilityBase::AbilityBase(unsigned int cost, double value, string nm): baseCost(cost), baseValue(value), name(nm) {}

double AbilityBase::calculateValue(unsigned int level)
{
    return baseValue*level*level;
}

unsigned int AbilityBase::calculateCost(unsigned int level)
{
    return baseCost*level*level;
}

string AbilityBase::getName()
{
    return name;
}

//////////////////////////////////////

AbilityBaseLinear::AbilityBaseLinear(unsigned int cost, double _a, double _b, string nm): AbilityBase(cost, 0.0, nm), a(_a), b(_b) {}

double AbilityBaseLinear::calculateValue(unsigned int level)
{
    return a*level+b;
}
