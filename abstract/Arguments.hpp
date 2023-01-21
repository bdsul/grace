#ifndef _Arguments_HPP_
#define _Arguments_HPP_

// Include system libraries
#include <cxxopts.hpp>

// Command-line arguments abstract class
class Arguments
{
public:
    // No constructor as class doesn't need initialisation
    virtual ~Arguments() = 0; // Declare as pure virtual to prevent instantiation

    // Derived class does not need to have an implementation
    virtual void addArguments(cxxopts::Options &){};
    virtual void parseArguments(cxxopts::ParseResult &){};
};

// Declare inline destructor to prevent linkage errors
inline Arguments::~Arguments(){};

#endif