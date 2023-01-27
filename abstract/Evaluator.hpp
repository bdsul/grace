#ifndef _EVALUATOR_HPP_
#define _EVALUATOR_HPP_

// Include system libraries
#include <memory>

// Include abstract classes
#include "Arguments.hpp"
#include "Settings.hpp"
#include "RNG.hpp"
#include "Population.hpp"

// Evaluator abstract class
template <class POPULATIONTYPE>
class Evaluator : public Arguments, public Settings, public RNG
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Evaluator() = 0; // Declare as pure virtual to prevent instantiation

    // Define pure virtual methods that derived classes must implement
    virtual bool evaluate(POPULATIONTYPE &population) = 0;
};

// Declare inline destructor to prevent linkage errors
template <class POPULATIONTYPE>
inline Evaluator<POPULATIONTYPE>::~Evaluator(){};

#endif