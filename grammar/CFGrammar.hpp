#ifndef _CFGRAMMAR_H_
#define _CFGRAMMAR_H_

#include <vector>
#include <string>
#include <limits.h>

#include "Grammar.hpp"
#include "../util/DerivationTree.hpp"

typedef std::vector<std::shared_ptr<CFRule>> CFRules;

// TODO: This class needs needs the BNF parsing to be refactored
class CFGrammar : public Grammar<CFRules>
{
public:
    CFGrammar();                  // Default constructor
    CFGrammar(const CFGrammar &); // Copy constructor
    virtual ~CFGrammar();         // Destructor

    // BNF File read
    bool readBNFFile(const char *);
    bool readBNFFile(const std::string &);
    bool readBNFString(const char *);
    bool readBNFString(const std::string &);
    bool addBNFString(const char *);
    bool addBNFString(const std::string &);

    // Pretty print BNF
    void outputBNF(std::ostream &) const;

    // Get/Set methods
    Grammar<CFRules>::SymbolPointer getStartSymbol() const;
    bool setStartSymbol(const Symbol &);

    CFRule *getStartRule() const;
    CFRule *findRule(const Symbol);

private:
    bool isRecursive(Choice::Symbols &, CFRule &);

    void updateRuleFields();
    void clearRuleFields();
};

#endif
