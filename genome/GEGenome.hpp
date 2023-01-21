#ifndef _GEGENOME_HPP_
#define _GEGENOME_HPP_

// Include system libraries
#include <vector>
#include <string>

// Include interface
#include "../interface/IGenome.hpp"

// Include composition classes
#include "../grammar/CFGrammar.hpp"

// This class implements...
class GEGenome : public IGenome
{
public:
    GEGenome() : phenotype(""), // Default Constructor - Initialise member variables
                 effectiveSize(0),
                 isPhenotypeValid(false),
                 isEvaluated(false){};

    GEGenome(GEGenome &copy) // Copy constructor
    { 
        genotype = copy.genotype;
        phenotype = copy.phenotype;
        grammar = copy.grammar;
        derivationTree = copy.derivationTree;
        effectiveSize = copy.effectiveSize;
        isPhenotypeValid = copy.isPhenotypeValid;
        isEvaluated = copy.isEvaluated;
    }

    virtual ~GEGenome(){}; // Destructor

public:
    // Member variables
    std::vector<unsigned int> genotype;
    std::string phenotype;
    CFGrammar grammar;
    DerivationTree derivationTree;
    unsigned int effectiveSize;
    bool isPhenotypeValid; // Used to indicate if the genotype has been modified or the mapping has failed
    bool isEvaluated;      // Used to skip mapping & evaluation if the genotype hasn't changed
};

#endif