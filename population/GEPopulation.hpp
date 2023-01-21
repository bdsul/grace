#ifndef _GEPOPULATION_HPP_
#define _GEPOPULATION_HPP_

// Include system libraries
#include <vector>
#include <memory>

// Include abstract classes
#include "../abstract/Population.hpp"

template <class GENOMETYPE>
class GEPopulation : public Population
{
public:
    GEPopulation(){}; // Default constructor
    virtual ~GEPopulation(){}; // Destructor

    // Define types to simplify expressions
    using GenomeType = GENOMETYPE;

    // Use shared pointers to manage memory
    std::vector<std::shared_ptr<GENOMETYPE>> individuals;
};

#endif
