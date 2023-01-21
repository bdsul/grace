#ifndef _GEPOPULATION_HPP_
#define _GEPOPULATION_HPP_

// Include system libraries
#include <vector>
#include <memory>

// Include interfaces
#include "../interface/IPopulation.hpp"

template <class GENOMETYPE>
class GEPopulation : public IPopulation
{
public:
    GEPopulation(){};
    virtual ~GEPopulation(){};

    // Define types to simplify expressions
    using GenomeType = GENOMETYPE;

    // Use shared pointers to manage memory
    std::vector<std::shared_ptr<GENOMETYPE>> individuals;
};

#endif
