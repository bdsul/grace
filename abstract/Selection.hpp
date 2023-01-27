#ifndef _SELECTION_HPP_
#define _SELECTION_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Selection abstract class
template <class POPULATIONTYPE>
class Selection : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Selection() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool select(POPULATIONTYPE &population, POPULATIONTYPE &parents) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Selection<POPULATIONTYPE>::~Selection(){};

#endif