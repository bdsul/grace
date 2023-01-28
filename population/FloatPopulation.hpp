#ifndef _FLOATPOPULATION_HPP_
#define _FLOATPOPULATION_HPP_

// Include system libraries
#include <vector>
#include <memory>
#include <type_traits>

#include "../genome/FloatGenome.hpp"

// Include abstract classes
#include "../abstract/Population.hpp"

class FloatPopulation : public Population<FloatGenome>
{
public:
    // Declare types to simplify expressions
    using GenomeType = Population<FloatGenome>::GenomeType;
    using GenomePointer = Population<FloatGenome>::GenomePointer;
    using Individuals = Population<FloatGenome>::Individuals;

    FloatPopulation(){}; // Default constructor
    virtual ~FloatPopulation(){}; // Destructor
};

#endif
