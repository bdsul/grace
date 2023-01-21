#ifndef _CFRULE_CPP_
#define _CFRULE_CPP_

// Include system libraries
#include <iostream>
#include <limits.h>

// Include header file
#include "CFRule.hpp"

// Default constructor
CFRule::CFRule() : Rule<CFRuleLHS>()
{
}

// Copy constructor
CFRule::CFRule(const CFRule &copy) : Rule<CFRuleLHS>(copy)
{
    // Copy lhs
    lhs = copy.lhs;
}

// Destructor
CFRule::~CFRule()
{
}

#endif
