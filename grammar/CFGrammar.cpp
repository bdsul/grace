#ifndef _CFGRAMMAR_CPP_
#define _CFGRAMMAR_CPP_

// Include system libraries
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <stack>
#include <fstream>
#include <string>

// Include header file
#include "CFGrammar.hpp"

// Default constructor
CFGrammar ::CFGrammar()
{
}

// Copy constructor
CFGrammar ::CFGrammar(const CFGrammar &copy) : Grammar(copy)
{
}

// Destructor
CFGrammar ::~CFGrammar()
{
}

// BNF file parsing
bool CFGrammar ::readBNFFile(const char *filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "Could not open grammar file " << filename << ".\nExecution aborted.\n";
        exit(1);
    }
    char buffer[1024];
    std::string program;
    while (!file.eof())
    {
        file.read(buffer, 1024);
        program.append(buffer, file.gcount());
    }
    file.close();
    program += "\n";
    return readBNFString(program.c_str());
}

bool CFGrammar ::readBNFFile(const std::string &filename)
{
    return readBNFFile(filename.c_str());
}

// TODO: This parser needs refactoring. Code taken from LibGE.
//       Modified to take a std::string instead of a char*.
bool CFGrammar ::readBNFString(const char *stream)
{
    rules.clear();                 // Delete the current grammar, if any
    CFRule newRule;                // Used to create new rules for grammar
    bool insertRule = false;       // If newRule is to be inserted onto grammar
    CFRule *currentRule = nullptr; // Used in pass 2 to add choice to current rule
    Choice newChoice;              // Used to create new choice for grammar
    Symbol newSymbol;              // Used to create new symbols for grammar
    Symbol newTokenSeparator;      // Used to create token separators for grammar
    unsigned int stream_size = strlen(stream);
    unsigned int ii;              // Stream iterator/position
    unsigned int jj;              // Working variables
    char currentChar;             // Current char of input
    bool skip = false;            // SKip an iteration on parser (for escaped newlines)
    bool quoted = false;          // If current char is quoted
    bool non_terminal = false;    // If current text is a non-terminal symbol
    char separated = 0;           // If there was a separator between previous token and current one
    Choice::Symbols nonTerminals; // Contains pointers to all defined non-terminals - TODO: Is this required?
    Choice::Symbols::iterator nonTerminalsIt;
    std::string currentBuffer; // Buffer used to add new symbols to grammar
    currentBuffer.reserve(stream_size);

    // States of parser
    enum parserStates
    {
        START,
        START_RULE,
        LHS_READ,
        CHOICE,
        START_OF_LINE
    };

    unsigned int state = START; // Set state of parser

    // Read in BNF File
    // First pass creates the ...
    // Second pass ...
    for (int pass = 1; pass <= 2; pass++)
    {
        ii = jj = 0;
        while (ii <= stream_size)
        {
            if (ii < stream_size)
            {
                currentChar = stream[ii];

                // Handle Comments
                while (currentChar == '#')
                {
                    // comment in the grammar; the while loop looks for comments
                    //  on consecutive lines
                    while (ii < stream_size && !((currentChar = stream[ii++]) == '\n' || currentChar == '\r'))
                        ;
                    // Ignore everything until the end of line or end of file,
                    //  whichever is earlier
                    if (ii == stream_size)
                        // As in the 'else' condition below the while loop,
                        // simulate presence of endl at end of grammar
                        // TODO: Why is there an endline at the end of the grammar?
                        currentChar = '\n';
                    else
                        currentChar = stream[ii]; // read the next character and work away.
                }
            }
            else
            {
                // Simulate presence of endl at end of grammar
                currentChar = '\n'; // TODO: Is this required?
            }

            // Handle escape sequence
            if (stream[ii] == '\\')
            {
                // Go to next character
                ++ii;

                // Error-handing - TODO: Tidy this up
                if (ii >= stream_size)
                {
                    // Escape sequence as last char is invalid
                    std::cerr << "Error in line 218. Exiting...";
                    exit(1);
                    // ERROR_IN_GRAMMAR;
                }
                else if ((non_terminal) && (stream[ii] != '\n'))
                {
                    // Only escaped newline allowed inside non-terminal
                    std::cerr << "ERROR: Escape sequence detected inside non-terminal symbol.\n";
                    exit(1);
                    // ERROR_IN_GRAMMAR;
                }

                // Add escaped character to currentChar
                switch (stream[ii])
                {
                case '\'':
                    currentChar = '\''; // Single quote
                case '\"':
                    currentChar = '\"'; // Double quotes
                case '\\':
                    currentChar = '\\'; // Backslash
                    currentChar = '\0'; // Null character
                case 'a':
                    currentChar = '\a'; // Audible bell
                case 'b':
                    currentChar = '\b'; // Backspace
                case 'f':
                    currentChar = '\f'; // Form-feed
                case 'n':
                    currentChar = '\n'; // Newline
                case 'r':
                    currentChar = '\r'; // Carriage return
                case 't':
                    currentChar = '\t'; // Horizontal tab
                case 'v':
                    currentChar = '\v'; // Vertical tab
                case '\n':
                    skip = true; // Escaped newline
                case '\r':
                    skip = true; // Escaped DOS return
                default:
                    currentChar = stream[ii]; // TODO: Should this give a warning?
                }

                // More error handling - Why do this?
                if (stream[ii] == '\r')
                {
                    // Escaped DOS return
                    // Ignore newline
                    skip = true;
                    if (stream[++ii] != '\n')
                    {
                        std::cerr << "ERROR: \\r character not followed by \\n.\n";
                        exit(1);
                        // ERROR_IN_GRAMMAR;
                    }
                }

                // Check if we need to create a terminal symbol
                if (skip == false && (pass > 1))
                {
                    if (currentBuffer.empty())
                    {
                        newSymbol.setType(Symbol::TerminalSymbol);
                    }
                    currentBuffer += currentChar;
                }
            }
            else
            {
                switch (state)
                {
                // Start of Rule
                case (START):
                    if (currentChar == '\r')
                    {
                        break; // Ignore DOS newline first char
                    }
                    switch (currentChar)
                    {
                    case ' ':  // Ignore whitespaces
                    case '\t': // Ignore tabs
                    case '\n': // Ignore newlines
                        break;
                    case '<': // START OF RULE
                        newSymbol.setType(Symbol::NonTerminalSymbol);
                        currentBuffer += currentChar;
                        state = START_RULE;
                        break;
                    default: // Illegal
                        std::cerr << "ERROR: Character '" << currentChar << "' caused an error.\n";
                        exit(1);
                    }
                    break;
                // Read LHS Non-terminal
                case (START_RULE): // Read the lhs Non-terminal symbol
                    if (currentChar == '\r')
                    {
                        break; // Ignore DOS newline first char
                    }
                    switch (currentChar)
                    {
                    case '\n': // Newlines are illegal here
                        std::cerr << "ERROR: Newline inside non-terminal.\n";
                        // TODO: Why does this break and not exit?
                        exit(1); // Exit with failure
                        // break;
                    case '>': // Possible end of non-terminal symbol
                        currentBuffer += currentChar;
                        // Set the string of the newSymbol
                        newSymbol.setValue(currentBuffer.c_str());

                        if (pass == 1)
                        {
                            // First pass
                            // Check if new symbol definition
                            insertRule = !(findRule(newSymbol));
                            if (insertRule)
                            {
                                // Create new rule for symbol
                                // newRule.clear();

                                // Add new symbol to Heap
                                // newRule.lhs = availableSymbols.add(newSymbol);
                                newRule.lhs = std::make_shared<Symbol>(newSymbol);
                                // newRule.lhs.push_back(new Symbol(newSymbol));
                                // newRule.lhs.push_back(newSymbol);
                                // Add to grammar˙
                                // nonTerminals.push_back(newRule.lhs.back());
                            }
                            else
                            {
                                // Existing symbol, do nothing
                            }
                        }
                        else
                        {
                            // Second pass
                            // Point currentRule to previously defined rule
                            // TODO: What is this doing?
                            if (!(currentRule = findRule(newSymbol)))
                            {
                                std::cerr << "ERROR: Grammar changed between parser passes.\n";
                                exit(1);
                                // ERROR_IN_GRAMMAR;
                            }
                        }
                        // Reset the buffer
                        currentBuffer.clear();
                        // lhs for this rule has been read
                        state = LHS_READ;
                        break;
                    default: // Check for non-escaped special characters
                        if (((currentChar == '"') || (currentChar == '|') || (currentChar == '<')))
                        {
                            std::cerr << "ERROR: Invalid character '" << currentChar << "' inside non-terminal symbol.\n";
                            exit(1);
                            // ERROR_IN_GRAMMAR;
                        }
                        currentBuffer += currentChar;
                    }
                    break;
                case (LHS_READ): // Must read ::= token
                    if (currentChar == '\r')
                    {
                        break; // Ignore DOS newline first char
                    }
                    switch (currentChar)
                    {
                    case ' ':  // Ignore whitespaces
                    case '\t': // Ignore tabs
                    case '\n': // Ignore newlines
                        break;
                    case ':': // Part of ::= token
                        currentBuffer += currentChar;
                        break;
                    case '=': // Should be end of ::= token
                        currentBuffer += currentChar;
                        if (currentBuffer != "::=")
                        {
                            // Something other than ::= was read
                            std::cerr << "ERROR: Token '" << currentBuffer << "' caused an error\n";
                            exit(1);
                            // ERROR_IN_GRAMMAR;
                        }
                        // Reset the buffer
                        currentBuffer.clear();
                        // START OF CHOICE
                        newChoice.symbols.clear();
                        state = CHOICE;
                        break;
                    default: // Illigal
                        std::cerr << "ERROR: Character '" << currentChar << "' caused an error.\n";
                        exit(1);
                        // ERROR_IN_GRAMMAR;
                    }
                    break;
                case (CHOICE): // Read everything until | token or \n, or EOL
                    if (currentChar == '\r')
                    {
                        break; // Ignore DOS newline first char
                    }
                    if (pass == 1)
                    {
                        if (currentChar == '\n')
                            state = START_OF_LINE;
                    }
                    else
                        switch (currentChar)
                        {
                        case '|': // Possible end of production
                            if (quoted)
                            {
                                // Normal character
                                currentBuffer += currentChar;
                                break;
                            }
                        case '\n': // End of production (and possibly rule)
                            // Reset separator marker
                            separated = 0;
                            if (currentBuffer.length() || !newChoice.symbols.size())
                            {
                                // There is a symbol to add
                                if (!currentBuffer.length())
                                {
                                    // No symbol exists; create terminal empty symbol
                                    newSymbol.setType(Symbol::TerminalSymbol);
                                }
                                if (non_terminal)
                                {
                                    // Current non-terminal symbol isn't finished
                                    std::cerr << "ERROR: Symbol '" << currentBuffer << "' caused an error.\n";
                                    exit(1);
                                    // ERROR_IN_GRAMMAR;
                                }
                                newSymbol.setValue(currentBuffer.c_str());
                                if (newSymbol.getType() == Symbol::NonTerminalSymbol)
                                {
                                    // Find rule that defines this symbol
                                    CFRule *tempRule = findRule(newSymbol);
                                    if (tempRule)
                                    {
                                        // TODO: This makes no sense??
                                        newChoice.symbols.push_back(tempRule->lhs);
                                    }
                                    else
                                    {
                                        // Undefined symbol, insert anyway
                                        // newChoice.push_back(new Symbol(newSymbol));
                                        // TODO: WTF is happening here? The symbols need to be defined!
                                        // newChoice.symbols.push_back(availableSymbols.add(newSymbol));
                                        newChoice.symbols.push_back(std::make_shared<Symbol>(newSymbol));
                                    }
                                }
                                else
                                {
                                    // Add terminal symbol
                                    // newChoice.push_back(new Symbol(newSymbol));
                                    newChoice.symbols.push_back(std::make_shared<Symbol>(newSymbol));
                                    // newChoice.symbols.push_back(availableSymbols.add(newSymbol));
                                }
                                // Reset the symbol
                                newSymbol.setValue("");
                            }
                            else
                            {
                                // Empty token; ignored
                                ;
                            }
                            // END OF PRODUCTION

                            // Add production to current rule
                            currentRule->rhs.push_back(newChoice);
                            // Reset the buffer
                            currentBuffer.clear();
                            if (currentChar == '\n')
                                state = START_OF_LINE;
                            else
                            {
                                // START OF PRODUCTION
                                newChoice.symbols.clear();
                            }
                            break;
                        case '<':        // Possible start of non-terminal symbol
                        case '>':        // Possible end of non-terminal symbol
                        case ' ':        // Possible token separator
                        case '\t':       // Possible token separator
                            if ((quoted) // Normal character
                                || (((currentChar == ' ') || (currentChar == '\t')) && (non_terminal)))
                            {
                                // Spaces inside non-terminals are accepted
                                currentBuffer += currentChar;
                                if (!non_terminal)
                                    newSymbol.setType(Symbol::TerminalSymbol);
                                break;
                            }
                            if (currentChar == '>')
                            {
                                // This is also the end of a non-terminal symbol
                                currentBuffer += currentChar;
                                non_terminal = false;
                            }
                            if (currentBuffer.length())
                            {
                                if (non_terminal)
                                {
                                    // Current non-terminal symbol isn't finished
                                    std::cerr << "ERROR: Symbol " << currentBuffer << "' caused an error.\n";
                                    exit(1);
                                    // ERROR_IN_GRAMMAR;
                                }
                                if ((currentChar == ' ') || (currentChar == '\t'))
                                {
                                    // Token separator
                                    separated = 1;
                                }
                                newSymbol.setValue(currentBuffer.c_str());
                                if (newSymbol.getType() == Symbol::NonTerminalSymbol)
                                {
                                    // Find rule that defines this symbol
                                    CFRule *tempRule = findRule(newSymbol);
                                    if (tempRule)
                                    {
                                        newChoice.symbols.push_back(tempRule->lhs);
                                    }
                                    else
                                    {
                                        // Undefined symbol, insert anyway
                                        // newChoice.push_back(new Symbol(newSymbol));
                                        newChoice.symbols.push_back(std::make_shared<Symbol>(newSymbol));
                                        // newChoice.symbols.push_back(availableSymbols.add(newSymbol));
                                    }
                                }
                                else
                                {
                                    // Add terminal symbol
                                    // newChoice.push_back (new Symbol(newSymbol));
                                    newChoice.symbols.push_back(std::make_shared<Symbol>(newSymbol));
                                    // newChoice.symbols.push_back(availableSymbols.add(newSymbol));
                                }
                                // Reset the symbol
                                newSymbol.setValue("");
                            }
                            else
                            {
                                // Empty buffer
                                if (((currentChar == ' ') || (currentChar == '\t'))
                                    // Token separator
                                    && (!newChoice.symbols.empty()))
                                {
                                    // Probably a token separator after a non-terminal symbol
                                    separated = 1;
                                }
                            }
                            // Reset the buffer
                            currentBuffer.clear();
                            if (currentChar == '<')
                            {
                                // This is also the start of a non-terminal symbol
                                // Special case; must create new Symbol here
                                newSymbol.setValue("");
                                newSymbol.setType(Symbol::NonTerminalSymbol);
                                currentBuffer += currentChar;
                                non_terminal = true; // Now reading a non-terminal symbol
                                if (separated)
                                {
                                    // Insert a token separator
                                    separated = 0;
                                    newTokenSeparator.setValue(" ");
                                    // newTokenSeparator = " ";
                                    newTokenSeparator.setType(Symbol::TerminalSymbol);
                                    // newChoice.push_back(new Symbol(newTokenSeparator));
                                    newChoice.symbols.push_back(std::make_shared<Symbol>(newTokenSeparator));

                                    // newChoice.symbols.push_back(availableSymbols.add(newTokenSeparator));
                                }
                            }
                            break;
                        default: // Add character to current buffer
                            if (separated)
                            {
                                // Insert a token separator
                                separated = 0;
                                newTokenSeparator.setValue(" ");
                                // newTokenSeparator = " ";
                                newTokenSeparator.setType(Symbol::TerminalSymbol);
                                // newChoice.push_back(new Symbol(newTokenSeparator));
                                newChoice.symbols.push_back(std::make_shared<Symbol>(newTokenSeparator));
                                // newChoice.symbols.push_back(availableSymbols.add(newTokenSeparator));
                            }
                            if (currentChar == '"')
                            {
                                // Start (or end) quoted section
                                quoted = !quoted;
                                newSymbol.setType(Symbol::TerminalSymbol);
                                break;
                            }
                            if (!currentBuffer.length())
                            {
                                newSymbol.setType(Symbol::TerminalSymbol);
                            }
                            currentBuffer += currentChar;
                        }
                    break;
                case (START_OF_LINE):
                    if (currentChar == '\r')
                    {
                        break; // Ignore DOS newline first char
                    }
                    switch (currentChar)
                    {
                    case ' ':  // Ignore whitespaces
                    case '\t': // Ignore tabs
                    case '\n': // Ignore newlines
                        break;
                    case '|': // Start of new production
                        state = CHOICE;
                        if (pass == 2)
                        {
                            // START OF CHOICE
                            newChoice.symbols.clear();
                        }
                        break;
                    case '<': // Start of lhs non-terminal symbol
                        // END OF RULE
                        if (pass == 1)
                        {
                            // Add current rule
                            if (insertRule)
                            {
                                rules.push_back(std::make_shared<CFRule>(newRule));
                            }
                        }
                        // START OF RULE
                        newSymbol.setType(Symbol::NonTerminalSymbol);
                        currentBuffer += currentChar;
                        state = START_RULE;
                        break;
                    default: // Illigal
                        std::cerr << "ERROR: Detected line starting with terminal symbol.\n";
                        exit(1);
                        // ERROR_IN_GRAMMAR;
                    }
                    break;
                default: // Impossible error, quit the program now!
                    std::cerr << "Internal error.\nPlease report error 'Impossible state in CFGrammar parser'\n";
                    exit(1);
                }
            }
            skip = false;
            ii++;
        }
        // END OF PASS
        if (state != START_OF_LINE)
        {
            // This must be the state of the parser
            std::cerr << "Parsing error reading grammar.\n";
            exit(1);
            // ERROR_IN_GRAMMAR;
        }
        if (pass == 1)
        {
            // Add current rule
            if (insertRule)
            {
                rules.push_back(std::make_shared<CFRule>(newRule));
            }
        }
    }
    updateRuleFields();
    setValidGrammar(true);

    // TODO: Set default start symbol to the first rule
    setStartSymbol(*rules.begin()->get()->lhs);

    // genotype2Phenotype();
    return true;
}

bool CFGrammar ::readBNFString(const std::string &stream)
{
    return readBNFString(stream.c_str());
}

bool CFGrammar ::addBNFString(const char *stream)
{
    // TODO: Slow implementation.
    // Copy current grammar
    // Rules grammarCopy(*this);
    // Create appended grammar string
    std::ostringstream newGrammarStream;
    outputBNF(newGrammarStream);
    std::string newGrammar(newGrammarStream.str());
    newGrammar.append(stream);
    // Read new grammar
    if (!readBNFString(newGrammar.c_str()))
    {
        // Restore previous state of Mapper
        readBNFString(newGrammarStream.str().c_str());
        return false;
    }
    return true;
}

bool CFGrammar ::addBNFString(const std::string &stream)
{
    return addBNFString(stream.c_str());
}

// TODO: This pretty print could do with refactoring
void CFGrammar ::outputBNF(std::ostream &stream) const
{
    if (!getValidGrammar())
    {
        return;
    }
    CFRules::const_iterator ruleIt = rules.begin();

    while (ruleIt != rules.end())
    {
        stream << ruleIt->get()->lhs->getValue() << " ::= ";

        // Go through each choice
        CFRule::Choices::const_iterator choiceIt = ruleIt->get()->rhs.begin();
        while (choiceIt != ruleIt->get()->rhs.end())
        {
            // go through symbols
            Choice::Symbols::const_iterator symbIt = choiceIt->symbols.begin();
            while (symbIt != choiceIt->symbols.end())
            {
                stream << (*symbIt)->getValue();
                // get next symbol
                symbIt++;
            }
            // get next production
            choiceIt++;
            if (choiceIt != ruleIt->get()->rhs.end())
            {
                stream << " | ";
            }
        }
        stream << '\n';
        // get next rule
        ruleIt++;
    };
}

/* ---- Returns the address of the rule ---- */
// defining the argument non-terminal symbol,
// if it exists; otherwise returns NULL.

// TODO: should the Rule pointer be const?
CFRule *CFGrammar::findRule(const Symbol symbol)
{
    CFRules::iterator ruleIt = rules.begin();

    // Look to see if there is a rule already declared
    for (ruleIt = rules.begin(); ruleIt != rules.end(); ++ruleIt)
    {
        if (*(ruleIt->get()->lhs) == symbol)
        {
            // Rule found, return address
            return ruleIt->get();
        }
    }

    // No rule was found, return a NULL pointer
    return nullptr;
}

/* --- Returns the calculated recursive ---- */
// nature of the Rule passed as argument, and updates its minimum
// mapping depth (minimumDepth)
bool CFGrammar ::isRecursive(Choice::Symbols &visitedRules, CFRule &currentRule)
{
    CFRule::Choices::iterator choiceIt;
    Choice::Symbols::iterator symbIt;
    Choice::Symbols::iterator visitedRulesIt = visitedRules.begin();
    CFRule *symbolRulePtr; // Const ptr as we aren't modifying the object

    bool result;
    // DON'T DO IT - otherwise minimumDepth is not updated properly

    // Check if this is a recursive call to a previously visited rule
    for (visitedRulesIt = visitedRules.begin(); visitedRulesIt != visitedRules.end(); ++visitedRulesIt)
    {
        // Has this rule been visited before?
        if (*visitedRulesIt == currentRule.lhs)
        {
            currentRule.setRecursive(true);
            return true;
        }
    }

    // Go through each choice in the rule
    for (choiceIt = currentRule.rhs.begin(); choiceIt != currentRule.rhs.end(); ++symbIt)
    {
        choiceIt->setMinimumDepth(0);
        symbIt = choiceIt->symbols.begin();
        // Go through each symbol in the production
        while (symbIt != choiceIt->symbols.end())
        {
            if ((*symbIt)->getType() == Symbol::NonTerminalSymbol)
            {
                // Symbol is non-terminal, possible recursion
                symbolRulePtr = findRule(**symbIt); // Find defining rule
                if (symbolRulePtr)
                {
                    // Symbol is defined
                    // Recursive call
                    visitedRules.push_back(currentRule.lhs);
                    result = isRecursive(visitedRules, *symbolRulePtr);
                    visitedRules.pop_back();

                    if (result == true)
                    {
                        // Production is recursive
                        // Mark production as recursive
                        choiceIt->setRecursive(true);
                        // Mark current rule also as recursive
                        currentRule.setRecursive(true);
                    }

                    // TODO: What does minimum depth mean here?
                    // Minimum Depth to fully map all Non-Terminal Symbols?
                    if (choiceIt->getMinimumDepth() < (symbolRulePtr->getMinimumDepth() + 1))
                    {
                        choiceIt->setMinimumDepth(symbolRulePtr->getMinimumDepth() + 1);
                    }
                }
                else
                {
                    // Non-defined non-terminal symbols are considered terminal symbols
                    if (choiceIt->getMinimumDepth() < 1)
                    {
                        choiceIt->setMinimumDepth(1);
                    }
                }
            }
            else
            {
                // Terminal symbol
                if (choiceIt->getMinimumDepth() < 1)
                {
                    choiceIt->setMinimumDepth(1);
                }
            }
            symbIt++;
        }

        if (currentRule.getMinimumDepth() > choiceIt->getMinimumDepth())
        {
            // Update rule minimumDepth
            currentRule.setMinimumDepth(choiceIt->getMinimumDepth());
        }
        choiceIt++;
    }

    return currentRule.getRecursive();
}

/* ---- Update recursive and minimumDepth fields ---- */
// for every Rule and Production in grammar.
void CFGrammar ::updateRuleFields()
{
    Choice::Symbols visitedRules;
    CFRules::iterator ruleIt = rules.begin();
    clearRuleFields();
    // Go through each rule in the grammar
    while (ruleIt != rules.end())
    {
        visitedRules.clear();
        // ruleIt->get()->setRecursive(isRecursive(visitedRules, &(*ruleIt)));
        ruleIt->get()->setRecursive(isRecursive(visitedRules, **ruleIt));
        ruleIt++;
    }
    // Second pass, to correctly update all recursive rules
    ruleIt = rules.begin();
    while (ruleIt != rules.end())
    {
        visitedRules.clear();
        // ruleIt->setRecursive(isRecursive(visitedRules, &(*ruleIt)));
        ruleIt->get()->setRecursive(isRecursive(visitedRules, **ruleIt));
        ruleIt++;
    }
}

/* ---- Update recursive and minimumDepth fields ---- */
// for every Rule and Production in grammar.
void CFGrammar ::clearRuleFields()
{
    CFRules::iterator ruleIt = rules.begin();
    // Reset minimum depths and recursion fields
    while (ruleIt != rules.end())
    {
        ruleIt->get()->setMinimumDepth(INT_MAX >> 1);
        ruleIt->get()->setRecursive(false);
        ruleIt++;
    }
}

/* ----  Return pointer to current start rule ---- */
CFRule *CFGrammar::getStartRule() const
{
    return &*rules.front();
    // return genome->rules.front();
}

/* ---- Return pointer to current start symbol ---- */
Grammar<CFRules>::SymbolPointer CFGrammar::getStartSymbol() const
{
    return startSymbol;

    // return rules[starterminalSymbol].lhs; // Context-Free
    //  return rules[starterminalSymbol].lhs.front(); // Context-Sensitive
}

/* ---- Change start symbol by index on Vector of rules ---- */
// bool CFGrammar ::setStartSymbol(const unsigned int index)
// {
// #if (DEBUG_LEVEL >= 2)
//     cerr << "'bool Grammar::setStartSymbol(const unsigned int)' called\n";
// #endif
//     // Check boundaries.
//     if (index < rules.size())
//     {
//         starterminalSymbol = index;
//         // Update phenotype.
//         // genotype2Phenotype();
//         // TODO: Invalidate the phenotype - How?

//         return true;
//     }
//     return false;
// }

/* ---- Change start symbol by symbol pointer ---- */
// TODO: Use std::make_shared to create start symbol
bool CFGrammar::setStartSymbol(const Symbol &newStartSymbol)
{
    int ii = 0;
    CFRules::iterator ruleIt = rules.begin();
    while (ruleIt != rules.end())
    {
        // Work by *pointer*.
        if (*ruleIt->get()->lhs == newStartSymbol)
        {
            // starterminalSymbol = ii;

            startSymbol = ruleIt->get()->lhs;
            // Update phenotype.
            // genotype2Phenotype();
            return true;
        }
        ruleIt++;
        ii++;
    }
    return false;
}

#endif
