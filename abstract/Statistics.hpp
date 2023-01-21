#ifndef _ISTATISTICS_HPP_
#define _ISTATISTICS_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "Random.hpp"
#include "Population.hpp"

// Statistics abstract class
class Statistics : public Arguments, public Settings, public Random
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Statistics() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool step(Population &population) = 0;
    virtual bool end(Population &population) = 0;
};

// Declare inline destructor to prevent linkage errors
inline Statistics::~Statistics(){};

#endif
