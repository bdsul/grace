#ifndef _INITIALISER_HPP_
#define _INITIALISER_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Initialiser abstract class
template <class POPULATIONTYPE>
class Initialiser : public Arguments, public Settings, public RNG //, public Settings
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Initialiser() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool initialise(POPULATIONTYPE &population) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Initialiser<POPULATIONTYPE>::~Initialiser(){};

#endif
