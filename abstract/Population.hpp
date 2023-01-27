#ifndef _POPULATION_HPP_
#define _POPULATION_HPP_

// Include abstract classes
#include "Genome.hpp"

// Population abstract class
template <class GENOMETYPE>
class Population
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Population() = 0; // Declare as pure virtual to prevent instantiation

    // Check that the templated parameter is derived from the Genome class
    static_assert(std::is_base_of<Genome, GENOMETYPE>());

    // Define types to simplify expressions
    using GenomeType = GENOMETYPE;
    using GenomePointer = std::shared_ptr<GenomeType>;
    using Individuals = std::vector<GenomePointer>;

    // Use shared pointers to manage memory
    Individuals individuals;
};

// Declare inline destructor to prevent linkage errors
template <class GENOMETYPE>
inline Population<GENOMETYPE>::~Population(){};

#endif