#ifndef _CHOICE_CPP
#define _CHOICE_CPP

#include <iostream>
#include <sstream>
#include <limits.h>

#include "Choice.hpp"

// Default constructor
Choice::Choice(const unsigned int newLength) : recursive(false),
                                               minimumDepth(INT_MAX >> 1)
{
}

// Copy constructor
Choice::Choice(const Choice &copy) : symbols(copy.symbols),
                                     recursive(copy.getRecursive()),
                                     minimumDepth(copy.getMinimumDepth())
{
}

// Destructor
Choice::~Choice()
{
}

// Get/Set methods
bool Choice::getRecursive() const
{
    return this->recursive;
}

void Choice::setRecursive(const bool newRecursive)
{
    this->recursive = newRecursive;
}

unsigned int Choice::getMinimumDepth() const
{
    return this->minimumDepth;
}

void Choice::setMinimumDepth(const unsigned int newMinimumDepth)
{
    this->minimumDepth = newMinimumDepth;
}

#endif