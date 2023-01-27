#ifndef _RNG_HPP_
#define _RNG_HPP_

// Include system libraries
#include <random>

// Random Number Generator abstract class
class RNG
{
public:
    RNG(){ // Default Constructor
        rng.seed(0);
    }

    RNG(RNG &copy){
        this->rng = copy.rng;
    }

    virtual ~RNG() = 0;  // Declare as pure virtual to prevent instantiation

    virtual void setRNGSeed(unsigned int seed){
        rng.seed(seed);
    };

    std::minstd_rand rng;
};

inline RNG::~RNG(){};

#endif