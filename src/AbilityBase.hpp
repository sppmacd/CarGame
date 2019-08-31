#pragma once

#include <string>

using namespace std;

// Ability class, contains equation functions for abilities.
// Default function is quadratic: f(level)=baseCost*level^2.
class AbilityBase
{
protected:
    unsigned int baseCost;
    double baseValue;
    string name; //Unlocalized name

public:
    virtual ~AbilityBase() {}

    AbilityBase(unsigned int cost, double value, string nm);

    // Calculate ability value from level. Function: baseValue*level^2
    virtual double calculateValue(unsigned int level);

    // Calculate cost of specified ability level. Function: baseCost*level^2 (same as powers)
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
    AbilityBaseLinear(unsigned int cost, double _a, double _b, string nm);

    // Calculate ability value from level. Function: a*level+b
    virtual double calculateValue(unsigned int level);
};
