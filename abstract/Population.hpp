#ifndef _POPULATION_HPP_
#define _POPULATION_HPP_

// Include abstract classes
#include "Genome.hpp"

// Population abstract class
class Population
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Population() = 0; // Declare as pure virtual to prevent instantiation
};

// Declare inline destructor to prevent linkage errors
inline Population::~Population(){};

#endif