#ifndef _REPLACEMENT_HPP_
#define _REPLACEMENT_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Replacement abstract class
template <class POPULATIONTYPE>
class Replacement : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Replacement() = 0;  // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool replace(POPULATIONTYPE &population, POPULATIONTYPE &children) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Replacement<POPULATIONTYPE>::~Replacement(){};

#endif


