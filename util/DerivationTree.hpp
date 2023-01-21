#ifndef _DERIVATIONTREE_HPP_
#define _DERIVATIONTREE_HPP_

// Include system libraries
#include <vector>
#include <memory>

// Include member headers
#include "../grammar/Symbol.hpp"

class DerivationTree
{
public:
    // Define new types to help readability
    using SymbolPointer = std::shared_ptr<Symbol>;
    using Children = std::vector<DerivationTree>;

    DerivationTree(const unsigned int = 1, const unsigned int = 1); // Default constructor
    DerivationTree(const SymbolPointer &, const unsigned int = 1, const unsigned int = 1); // Constructor with Symbol
    DerivationTree(const DerivationTree &); // Copy constructor
    ~DerivationTree(); // Destructor
    
    // Get/Set methods
    unsigned int getDepth() const;
    void setDepth(const unsigned int);
    unsigned int getCurrentLevel() const;
    void setCurrentLevel(const unsigned int);
    const SymbolPointer getData() const;
    void setData(const SymbolPointer &);
    const Children getChildren() const;
    void setChildren(const Children);

    // Print derivation tree to screen
    void printTree();

    // Public variables
    Children children;

private:
    // Private variables
    SymbolPointer data;
    unsigned int depth; // Track the maximum depth of this path
    unsigned int currentLevel; // Track the current level of this node
};

#endif
