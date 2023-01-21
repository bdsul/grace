#ifndef _CHOICE_HPP_
#define _CHOICE_HPP_

#include <vector>
#include "Symbol.hpp"

class Choice
{
public:
    Choice(const unsigned int = 0); // Default constructor
    Choice(const Choice &);         // Copy constructor
    ~Choice();                      // Destructor

    // Define new types to help readability
    using Symbols = std::vector<std::shared_ptr<Symbol>>;

    // Get/Set methods
    bool getRecursive() const;
    void setRecursive(const bool);
    unsigned int getMinimumDepth() const;
    void setMinimumDepth(const unsigned int);

    // Public variables
    Symbols symbols;

private:
    // Private variables
    bool recursive;
    unsigned int minimumDepth;
};

#endif
