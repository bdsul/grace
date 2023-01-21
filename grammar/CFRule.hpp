#ifndef _CFRULE_HPP_
#define _CFRULE_HPP_

// Include inherited classes
#include "Rule.hpp"

// Define LHS
typedef std::shared_ptr<Symbol> CFRuleLHS; // A single symbol

class CFRule : public Rule<CFRuleLHS>
{
public:
    CFRule();               // Default constructor
    CFRule(const CFRule &); // Copy constructor
    virtual ~CFRule();      // Destructor
};

#endif
