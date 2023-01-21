#ifndef _SYMBOL_CPP_
#define _SYMBOL_CPP_

// Include header
#include "./Symbol.hpp"

// Default constructor
Symbol::Symbol(const std::string newValue, const SymbolType newType) : value(newValue),
                                                                       type(newType)
{
}

// Copy constructor
Symbol::Symbol(const Symbol &copy) : value(copy.getValue()),
                                     type(copy.getType())
{
}

// Destructor
Symbol::~Symbol()
{
}

// Set/Get Methods
Symbol::SymbolType Symbol::getType() const
{
    return this->type;
}

void Symbol::setType(const Symbol::SymbolType newType)
{
    this->type = newType;
}

std::string Symbol::getValue() const
{
    return this->value;
}

void Symbol::setValue(const std::string newValue)
{
    this->value = newValue;
}

// Assignment operator
bool Symbol::operator==(const Symbol &newSymbol)
{
    if (this->getValue() != newSymbol.getValue())
    {
        return false;
    }
    if (this->getType() != newSymbol.getType())
    {
        return false;
    }
    return true;
}

#endif
