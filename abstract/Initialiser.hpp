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
class Initialiser : public Arguments, public Settings, public Random //, public Settings
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Initialiser() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool initialise(Population &population) = 0;
};

// Declare inline destructor to prevent linkage errors
inline Initialiser::~Initialiser(){};

#endif
