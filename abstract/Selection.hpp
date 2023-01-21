#ifndef _ISELECTION_HPP_
#define _ISELECTION_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "Random.hpp"
#include "Population.hpp"

// Selection abstract class
class Selection : public Arguments, public Settings, public Random
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Selection() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool select(Population &population, Population &parents) = 0;
};

// Declare inline destructor to prevent linkage errors
inline Selection::~Selection(){};

#endif