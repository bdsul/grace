#ifndef _MUTATION_HPP_
#define _MUTATION_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Mutation abstract class
template <class POPULATIONTYPE>
class Mutation : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Mutation() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool mutate(POPULATIONTYPE &population) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Mutation<POPULATIONTYPE>::~Mutation(){};

#endif
