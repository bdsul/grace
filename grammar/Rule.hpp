#ifndef _RULE_HPP_
#define _RULE_HPP_

// Include member classes
#include "Choice.hpp"

// Template rule class
template <typename LHS>
class Rule
{
public:
    Rule(bool recursive = false, unsigned int maximumDepth = INT_MAX >> 1); // Default constructor
    Rule(const Rule<LHS> &copy);                                            // Copy constructor
    virtual ~Rule();                                                        // Destructor

    // Define new types to help readability
    using Choices = std::vector<Choice>;

    // Get/Set methods
    bool getRecursive() const;
    void setRecursive(const bool);
    unsigned int getMinimumDepth() const;
    void setMinimumDepth(const unsigned int);

    // Public Variables
    LHS lhs;
    Choices rhs;

private:
    // Private Variables
    bool recursive;
    unsigned int minimumDepth;
};

// Default constructor
template <typename LHS>
Rule<LHS>::Rule(bool recursive, unsigned int maximumDepth)
    : recursive(recursive),
      minimumDepth(maximumDepth)
{
}

// Copy constructor
template <typename LHS>
Rule<LHS>::Rule(const Rule<LHS> &copy)
{
    // LHS copy should be implemented by the child
    rhs = copy.rhs;
    recursive = copy.recursive;
    minimumDepth = copy.minimumDepth;
}

// Destructor
template <typename LHS>
Rule<LHS>::~Rule() {}

// Get/Set methods
template <typename LHS>
bool Rule<LHS>::getRecursive() const
{
    return this->recursive;
}

template <typename LHS>
void Rule<LHS>::setRecursive(const bool newRecursive)
{
    this->recursive = newRecursive;
}

template <typename LHS>
unsigned int Rule<LHS>::getMinimumDepth() const
{
    return this->minimumDepth;
}

template <typename LHS>
void Rule<LHS>::setMinimumDepth(const unsigned int newMinimumDepth)
{
    this->minimumDepth = newMinimumDepth;
}

#endif