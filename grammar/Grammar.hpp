#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

// Include system libraries
#include <vector>
#include <string>
#include <any>

// Include member headers
#include "CFRule.hpp"

template <typename RULES>
class Grammar
{
public:
    Grammar();                // Default constructor
    Grammar(const Grammar &); // Copy constructor
    virtual ~Grammar();       // Destructor

    // Define new types to help readability
    using SymbolPointer = std::shared_ptr<Symbol>;

    //
    virtual bool readBNFFile(const char *) = 0;
    virtual bool readBNFFile(const std::string &) = 0;
    virtual void outputBNF(std::ostream &) const = 0;

    // Get/Set methods
    virtual SymbolPointer getStartSymbol() const = 0; // Returns pointer to start symbol
    virtual bool setStartSymbol(const Symbol &) = 0;
    void setValidGrammar(const bool);
    bool getValidGrammar() const;

    // Public variables
    RULES rules;

protected:
    // Protected variables
    bool validGrammar;
    SymbolPointer startSymbol;
};

// Default constructor
template <typename RULES>
Grammar<RULES>::Grammar() : validGrammar(false),
                            startSymbol(nullptr)
{
}

// Copy constructor
template <typename RULES>
Grammar<RULES>::Grammar(const Grammar &copy) : validGrammar(copy.getValidGrammar()),
                                               startSymbol(copy.getStartSymbol()),
                                               rules(copy.rules)
{
}

// Destructor
template <typename RULES>
Grammar<RULES>::~Grammar()
{
}

// Get/Set methods
template <typename RULES>
bool Grammar<RULES>::getValidGrammar() const
{
    return this->validGrammar;
}

template <typename RULES>
void Grammar<RULES>::setValidGrammar(const bool newValidGrammar)
{
    this->validGrammar = newValidGrammar;
}

#endif
