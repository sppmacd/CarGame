#pragma once

#include <string>

using namespace std;

// Ability class, contains equation functions for abilities.
// Default function is quadratic: f(level)=baseCost*level^2.
class AbilityBase
{
protected:
    // Cost at start ability level (0)
    unsigned int baseCost;
    
    // Value at start ability level (0). Used in many ways in derived classes.
    double baseValue;
    
    // The unlocalized name, will be added to "ability." while displaying in GUI.
    string name;

public:
    virtual ~AbilityBase() {}

    // %cost - The base cost
    // %value - The base value
    // %nm - Unlocalized name
    AbilityBase(unsigned int cost, double value, string nm);

    // Calculate ability value from level. Function: baseValue*level^2
    // %level - Ability level.
    virtual double calculateValue(unsigned int level);

    // Calculate cost of specified ability level. Function: baseCost*level^2 (same as powers)
    // %level - Ability level
    virtual unsigned int calculateCost(unsigned int level);

    // Return unlocalized name of ability.
    string getName();
};

// Linear ability function: f(level)=a*level+b
class AbilityBaseLinear : public AbilityBase
{
protected:
    double b;
    double a;
public:
    // %cost - Base cost
    // %_a - Multiplier
    // %_b - Offset
    // %nm - Unlocalized name
    AbilityBaseLinear(unsigned int cost, double _a, double _b, string nm);

    // Calculate ability value from level. Function: a*level+b
    // %level - Ability level
    virtual double calculateValue(unsigned int level);
};
