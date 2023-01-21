#ifndef _IINITIALISER_HPP_
#define _IINITIALISER_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "Random.hpp"
#include "Population.hpp"

// Initialiser abstract class
template <class POPULATIONTYPE>
class Initialiser : public Arguments, public Settings, public Random //, public Settings
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Initialiser() = 0; // Declare as pure virtual to prevent instantiation

    // Check that population type is correct
    static_assert(std::is_base_of<Population, POPULATIONTYPE>());

    // Define pure virtual methods that derived classes must implement
    virtual bool initialise(POPULATIONTYPE &population) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Initialiser<POPULATIONTYPE>::~Initialiser() {};

#endif
