#ifndef _GEMAPPER_HPP_
#define _GEMAPPER_HPP_

// Include abstract classes
#include "../../abstract/Mapper.hpp"
#include "../../grammar/CFGrammar.hpp"

// This template class provides mapping methods that work
// with all classes that inherit GEGenome
template <class POPULATIONTYPE>
class GEMapper : public Mapper<POPULATIONTYPE>
{
public:
    // Define new types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    GEMapper();           // Default constructor
    ~GEMapper() override; // Destructor

    // Command-line & settings file methods
    void addArguments(cxxopts::Options &) override;
    void parseArguments(cxxopts::ParseResult &) override;
    void parseSettings(INIReader &) override;

    // Implement pure virtual method from Mapper
    bool map(POPULATIONTYPE &population);

protected:
    // Available crossover methods
    bool mapper(POPULATIONTYPE &population);

    // Variables
    int maxWrappingEvents;
    int currentWrappingEvents;

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (GEMapper::*MapperMethod)(POPULATIONTYPE &population);
    MapperMethod method;

    // Work methods
    bool addChildrenNodes(DerivationTree &currentNode, GenomeType &genome, std::vector<unsigned int>::iterator &genotypeIt, bool buildDerivationTree);
    bool mapGenotypeToPhenotype(GenomeType &genome, const bool buildDerivationTree);
};

// Default constructor
template <class POPULATIONTYPE>
GEMapper<POPULATIONTYPE>::GEMapper()
    : method(&GEMapper::mapper),
      maxWrappingEvents(0),
      currentWrappingEvents(0){};

// Destructor
template <class POPULATIONTYPE>
GEMapper<POPULATIONTYPE>::~GEMapper(){};

// Settings file parsing
template <class POPULATIONTYPE>
void GEMapper<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get mapping method
    if (settings.HasValue("GEMapper", "Method"))
    {
        std::string method = settings.Get("GEMapper", "Method", "UNKNOWN");

        if (method == "mapper")
        {
            this->method = &GEMapper::mapper;
        }
        else
        {
            std::cout << "Error: Invalid GEMapper method name. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Get maximum wrapping event
    if (settings.HasValue("GEMapper", "MaximumWrappingEvents"))
    {
        this->maxWrappingEvents = settings.GetInteger("GEMapper", "MaximumWrappingEvents", -1);

        // Check that it isn't negative
        if (this->maxWrappingEvents < 0)
        {
            std::cout << "Error: Invalid GEMapper maximum wrapping events. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Add command-line arguments
template <class POPULATIONTYPE>
void GEMapper<POPULATIONTYPE>::addArguments(cxxopts::Options &options)
{
    options.add_options("GEMapper")("w,wrappingevents", "Maximum Wrapping Events", cxxopts::value<unsigned int>());
};

// Parse command-line arguments
template <class POPULATIONTYPE>
void GEMapper<POPULATIONTYPE>::parseArguments(cxxopts::ParseResult &results)
{
    if (results.count("maxwrappingevents"))
    {
        this->maxWrappingEvents = results["maxwrappingevents"].as<unsigned int>();

        if (this->maxWrappingEvents < 0)
        {
            std::cout << "Error: Invalid mapping maximum wrapping events argument. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool GEMapper<POPULATIONTYPE>::map(POPULATIONTYPE &population)
{
    return (this->*method)(population);
};

// Normal mapping method
template <class POPULATIONTYPE>
bool GEMapper<POPULATIONTYPE>::mapper(POPULATIONTYPE &population)
{
    // For each individual, call the mapping method
    for (std::shared_ptr<GenomeType> &individual : population.individuals)
    {
        mapGenotypeToPhenotype(*individual, true);
    }

    return true;
}

// Recursive function that adds the child nodes to the current node of the derivation tree
template <class POPULATIONTYPE>
bool GEMapper<POPULATIONTYPE>::addChildrenNodes(DerivationTree &currentNode, GenomeType &genome, std::vector<unsigned int>::iterator &genotypeIt, bool buildDerivationTree)
{
    // Find the rule for the given symbol
    Symbol const *currentSymbol = &*currentNode.getData();
    CFRule const *rulePtr = genome.grammar.findRule(*currentSymbol);

    // Safety Checks
    // Does the rule exist?
    if (!rulePtr)
    {
        std::cout << "Rule Exists!" << std::endl;
        return false;
    }

    // Do we have a recursive rule that consumes no codons?
    // TODO: Implement this check

    // Does the genotype have enough codons?
    if (genotypeIt == genome.genotype.end())
    {
        // Check if wrapping is allowed
        if (currentWrappingEvents < maxWrappingEvents)
        {
            // Point the iterator to the start of the genotype
            genotypeIt = genome.genotype.begin();

            // Increment the wrapping events count
            ++currentWrappingEvents;
        }
        else{
            // Let the user know that we ran out of codons. This can indicate a badly designed grammar
            std::cout << "Warning: Ran out of codons! Setting individual to invalid..." << std::endl;
            genome.isPhenotypeValid = false;
        }

    }

    // TODO: Check libGE's safety checks as some may be missing

    // Get the current codon
    unsigned int codon = *genotypeIt;
    Choice const *chosenChoice;

    // Is there more than one choice to choose from
    if (rulePtr->rhs.size() > 1)
    {
        // Choose one of the available choices
        chosenChoice = &rulePtr->rhs.at(codon % rulePtr->rhs.size());

        // Consume the codon
        ++genotypeIt;

        // Increase the effective size of the genome
        ++genome.effectiveSize;
    }
    else
    {
        // No codon is used when there is only one available choice. Continue...
        chosenChoice = &rulePtr->rhs.at(0);
    }

    // For each symbol in the current choice
    for (Choice::Symbols::const_iterator symbIt = chosenChoice->symbols.begin(); symbIt < chosenChoice->symbols.end(); ++symbIt)
    {
        // Is the symbol a terminal?
        if ((*symbIt)->getType() == Symbol::TerminalSymbol)
        {
            // Add terminal string to phenotype
            genome.phenotype.append((*symbIt)->getValue());

            // Add terminal to derivation tree
            currentNode.children.push_back(*symbIt);

            // Add current level of node
            DerivationTree *childNode = &currentNode.children.back();
            childNode->setCurrentLevel(currentNode.getCurrentLevel() + 1);

            // TODO: Check if this commented section is required
            // Propogate depth up
            // currentNode.setDepth(childNode->getCurrentLevel());

            // Continue mapping symbols
            continue;
        }

        // Symbol is a non-terminal, recursively call this function
        else
        {
            // Add terminal to derivation tree
            currentNode.children.push_back(*symbIt);
            DerivationTree *childNode = &currentNode.children.back();

            // Increase the current level of the child node
            childNode->setCurrentLevel(currentNode.getCurrentLevel() + 1);

            // Map child nodes
            bool wasMapSuccessful = addChildrenNodes(*childNode, genome, genotypeIt, false);

            // Return if the mapping failed to exit quickly
            if (!wasMapSuccessful)
            {
                return false;
            }
            else
            {
                // Mapped terminals will already be added to the phenotype & derivation tree
                continue;
            }
        }
    }
    return true;
}

template <class POPULATIONTYPE>
bool GEMapper<POPULATIONTYPE>::mapGenotypeToPhenotype(GenomeType &genome, const bool buildDerivationTree)
{
    // Perform safety checks

    // If the phenotype is already valid, we don't need to remap
    // This is used to prevent remapping genomes that were mapped in the initialiser
    // TODO: Add a setting to force remapping of all genomes
    if (genome.isPhenotypeValid == true)
    {
        return false;
    }

    // Is the grammar valid?
    if (!genome.grammar.getValidGrammar())
    {
        // Grammar invalid, return failure
        return false;
    }

    // Set effectiveSize to 0
    genome.effectiveSize = 0;

    // Clear the existing phenotype and invalidate the phenotype
    genome.phenotype.clear();
    genome.isPhenotypeValid = false;

    // Does the genome contain at least one codon?
    if (genome.genotype.size() < 1)
    {
        // We can't map with no codons, return failure
        return false;
    }

    // Reset the derivation tree
    genome.derivationTree.children.clear();
    genome.derivationTree.setData(genome.grammar.getStartSymbol());
    genome.derivationTree.setCurrentLevel(0);

    // TODO: Check is this is necessary
    // genome.derivationTree.setDepth(1);

    // Get a pointer to the start of the genotype
    // This is required to know which codon to look at while mapping the children nodes
    std::vector<unsigned int>::iterator genoIt = genome.genotype.begin();

    // Add all children nodes to the start symbol - This will fully map the individual
    bool wasMapSuccessful = addChildrenNodes(genome.derivationTree, genome, genoIt, false);

    // The genome is valid if the mapping was successful
    genome.isPhenotypeValid = wasMapSuccessful;

    // Return the mapping result
    return wasMapSuccessful;
}

#endif
