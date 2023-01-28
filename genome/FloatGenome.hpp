#ifndef _FLOATGENOME_HPP_
#define _FLOATGENOME_HPP_

// Include system libraries
#include <vector>
#include <string>

// Include abstract classes
#include "../genome/GEGenome.hpp"

// This class implements a single objective genome using a float score
class FloatGenome : public GEGenome
{
public:
    FloatGenome() : score(0.0){}; // Default constructor
    FloatGenome(FloatGenome &copy) : GEGenome(copy) { // Copy constructor
        score = copy.score;
    };
    ~FloatGenome(){}; // Destructor

public:
    // Member variable
    float score;
};

#endif