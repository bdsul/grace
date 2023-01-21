#ifndef _DERIVATIONTREE_CPP_
#define _DERIVATIONTREE_CPP_

#include "DerivationTree.hpp"

// Default constructor
DerivationTree::DerivationTree(const unsigned int newDepth, const unsigned int newCurrentLevel) : depth(newDepth),
                                                                                                  currentLevel(newCurrentLevel)
{
}

// Constructor with Symbol
DerivationTree::DerivationTree(const DerivationTree::SymbolPointer &newData, const unsigned int newCurrentLevel, const unsigned int newDepth) : data(newData),
                                                                                                                                                depth(newDepth),
                                                                                                                                                currentLevel(newCurrentLevel)
{
}

// Copy constructor
DerivationTree::DerivationTree(const DerivationTree &copy) : children(copy.getChildren()),
                                                             data(copy.getData()),
                                                             depth(copy.getDepth()),
                                                             currentLevel(copy.getCurrentLevel())
{
}

// Destructor
DerivationTree::~DerivationTree()
{
}

// Get/Set methods
unsigned int DerivationTree::getDepth() const
{
    return this->depth;
}

void DerivationTree::setDepth(const unsigned int newDepth)
{
    this->depth = newDepth;
}

unsigned int DerivationTree::getCurrentLevel() const
{
    return this->currentLevel;
}

void DerivationTree::setCurrentLevel(const unsigned int newCurrentLevel)
{
    this->currentLevel = newCurrentLevel;
}

const DerivationTree::Children DerivationTree::getChildren() const
{
    return this->children;
}

void DerivationTree::setChildren(const DerivationTree::Children newChildren)
{
    this->children = newChildren;
}

const DerivationTree::SymbolPointer DerivationTree::getData() const
{
    return this->data;
}

void DerivationTree::setData(const DerivationTree::SymbolPointer &newData)
{
    this->data = newData;
}

// Print derivation tree to screen
void DerivationTree::printTree()
{
    if (data->getType() == Symbol::TerminalSymbol)
    {
        // Indent by level
        for (int i = 0; i < this->currentLevel; ++i)
        {
            std::cout << "-";
        }

        std::cout << this->data->getValue() << std::endl;
    }
    for (DerivationTree child : children)
    {
        child.printTree();
    }
}

#endif
