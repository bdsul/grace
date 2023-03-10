#ifndef _CROSSOVER_HPP_
#define _CROSSOVER_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Crossover abstract class
template <class POPULATIONTYPE>
class Crossover : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Crossover() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool crossover(POPULATIONTYPE &parents, POPULATIONTYPE &children) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Crossover<POPULATIONTYPE>::~Crossover(){};

#endif