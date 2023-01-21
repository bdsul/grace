#ifndef _IREPLACEMENT_HPP_
#define _IREPLACEMENT_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "Random.hpp"
#include "Population.hpp"

// Replacement abstract class
class Replacement : public Arguments, public Settings, public Random
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Replacement() = 0;  // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool replace(Population &oldPopulation, Population &newPopulation) = 0;
};

// Declare inline destructor to prevent linkage errors
inline Replacement::~Replacement(){};

#endif


