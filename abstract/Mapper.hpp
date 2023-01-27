#ifndef _MAPPER_HPP_
#define _MAPPER_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Mapper abstract class
template <class POPULATIONTYPE>
class Mapper : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Mapper() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool map(POPULATIONTYPE &population) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Mapper<POPULATIONTYPE>::~Mapper(){};

#endif