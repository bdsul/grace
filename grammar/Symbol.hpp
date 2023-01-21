#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

// Include system libraries
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

class Symbol
{
public:
    // Define enum for terminal/non-terminal symbol
    enum SymbolType
    {
        NonTerminalSymbol,
        TerminalSymbol
    };

    Symbol(const std::string = "", const SymbolType = TerminalSymbol); // Default constructor
    Symbol(const Symbol &);                                            // Copy constructor
    virtual ~Symbol();                                                 // Destructor

    // Set/Get Methods
    SymbolType getType() const;
    void setType(const SymbolType);
    std::string getValue() const;
    void setValue(const std::string s);

    // Assigment operator
    bool operator==(const Symbol &);

private:
    SymbolType type;   // Is the symbol a terminal or non-terminal?
    std::string value; // TODO: Template this variable. Using std::string as default.
};

#endif
