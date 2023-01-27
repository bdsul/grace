#ifndef _EVOLUTIONARYALGORITHM_HPP_
#define _EVOLUTIONARYALGORITHM_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"

// Evolutionary Algorithm abstract class
class EvolutionaryAlgorithm : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~EvolutionaryAlgorithm() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual void evolve() = 0;
};

// Declare inline destructor to prevent linkage errors
inline EvolutionaryAlgorithm::~EvolutionaryAlgorithm(){};

#endif
