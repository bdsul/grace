#ifndef _STATISTICS_HPP_
#define _STATISTICS_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Statistics abstract class
template <class POPULATIONTYPE>
class Statistics : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Statistics() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool step(POPULATIONTYPE &population) = 0;
    virtual bool end(POPULATIONTYPE &population) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Statistics<POPULATIONTYPE>::~Statistics(){};

#endif
