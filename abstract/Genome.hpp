#ifndef _GENOME_HPP_
#define _GENOME_HPP_

// Genome abstract class
class Genome
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Genome() = 0; // Declare as pure virtual to prevent instantiation
};

// Declare inline destructor to prevent linkage errors
inline Genome::~Genome(){};

#endif