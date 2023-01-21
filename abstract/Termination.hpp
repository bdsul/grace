#ifndef _TERMINATION_HPP_
#define _TERMINATION_HPP_

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "Random.hpp"
#include "Population.hpp"

// Termination abstract class
class Termination : public Arguments, public Settings, public Random
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Termination() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool terminate(Population &population) = 0;
};

// Declare inline destructor to prevent linkage errors
inline Termination::~Termination(){};

#endif


