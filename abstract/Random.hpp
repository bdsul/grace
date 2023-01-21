#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

// Include system libraries
#include <random>

// Random Number Generator abstract class
class Random
{
public:
    Random(){ // Default Constructor
        rng.seed(0);
    }

    Random(Random &copy){
        this->rng = copy.rng;
    }

    virtual ~Random() = 0;  // Declare as pure virtual to prevent instantiation

    virtual void setRNGSeed(unsigned int seed){
        rng.seed(seed);
    };

    std::minstd_rand rng;
};

inline Random::~Random(){};

#endif