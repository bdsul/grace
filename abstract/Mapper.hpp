#ifndef _IMAPPER_HPP_
#define _IMAPPER_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "Random.hpp"
#include "Population.hpp"

// Mapper abstract class
class Mapper : public Arguments, public Settings, public Random
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Mapper() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool map(Population &population) = 0;
};

// Declare inline destructor to prevent linkage errors
inline Mapper::~Mapper(){};

#endif