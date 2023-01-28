#ifndef _GECROSSOVER_HPP_
#define _GECROSSOVER_HPP_

// Include abstract classes
#include "../../abstract/Crossover.hpp"

// This template class provides crossover methods that work
// with all classes that inherit GEGenome
template <class POPULATIONTYPE>
class GECrossover : public Crossover<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    GECrossover();           // Default constructor
    ~GECrossover() override; // Destructor

    // Command-line & settings file methods
    void addArguments(cxxopts::Options &) override;
    void parseArguments(cxxopts::ParseResult &) override;
    void parseSettings(INIReader &) override;

    // Implement pure virtual method from Crossover
    bool crossover(POPULATIONTYPE &parents, POPULATIONTYPE &children) override;

protected:
    // Available crossover methods
    bool fixedOnePoint(POPULATIONTYPE &parents, POPULATIONTYPE &children);

    // Variables
    float rate;

    // Work methods
    bool fixedOnePointGenome(const GenomeType &mom, const GenomeType &dad, GenomeType &child1, GenomeType &child2);

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (GECrossover::*CrossoverMethod)(POPULATIONTYPE &parents, POPULATIONTYPE &children);
    CrossoverMethod method;
};

// Default constructor
template <class POPULATIONTYPE>
GECrossover<POPULATIONTYPE>::GECrossover()
    : method(&GECrossover<POPULATIONTYPE>::fixedOnePoint),
      rate(0.9){};

// Destructor
template <class POPULATIONTYPE>
GECrossover<POPULATIONTYPE>::~GECrossover(){};

// Settings file parsing
template <class POPULATIONTYPE>
void GECrossover<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get crossover method
    if (settings.HasValue("GECrossover", "Method"))
    {
        std::string methodName = settings.Get("GECrossover", "Method", "UNKNOWN");

        if (methodName == "fixedOnePoint")
        {
            this->method = &GECrossover<POPULATIONTYPE>::fixedOnePoint;
        }
        else
        {
            std::cout << "Error: Invalid GECrossover method name. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Get crossover rate
    if (settings.HasValue("GECrossover", "Rate"))
    {
        float rate = settings.GetReal("GECrossover", "Rate", -1.0);
        if (rate >= 0 && rate <= 1)
        {
            this->rate = rate;
        }
        else
        {
            std::cout << "Error: Invalid GECrossover rate. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Add command-line arguments
template <class POPULATIONTYPE>
void GECrossover<POPULATIONTYPE>::addArguments(cxxopts::Options &options)
{
    options.add_options("GECrossover")("c,crossoverrate", "Crossover Rate", cxxopts::value<unsigned char>());
};

// Parse command-line arguments
template <class POPULATIONTYPE>
void GECrossover<POPULATIONTYPE>::parseArguments(cxxopts::ParseResult &results)
{
    if (results.count("crossoverrate"))
    {
        this->rate = results["crossoverrate"].as<float>();
        if (this->rate < 0.0 || this->rate > 1.0)
        {
            std::cout << "Error: Invalid GECrossover rate from command line. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool GECrossover<POPULATIONTYPE>::crossover(POPULATIONTYPE &parents, POPULATIONTYPE &children)
{
    return (this->*method)(parents, children);
}

// Fixed one point crossover
template <class POPULATIONTYPE>
bool GECrossover<POPULATIONTYPE>::fixedOnePoint(POPULATIONTYPE &parents, POPULATIONTYPE &children)
{
    // Check that we have parents
    if (parents.individuals.size() < 2)
    {
        std::cout << "Error: Parent population size is less than two. Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Take two parents at a time
    typename Individuals::iterator currentIndividualIt = parents.individuals.begin();
    while ((currentIndividualIt + 2) < parents.individuals.end())
    {
        const GenomeType &mom = **currentIndividualIt;
        const GenomeType &dad = **(currentIndividualIt + 1);

        GenomePointer child1 = std::make_shared<GenomeType>();
        GenomePointer child2 = std::make_shared<GenomeType>();

        // Perform the crossover
        fixedOnePointGenome(mom, dad, *child1, *child2);

        // Copy the grammar to the children
        child1->grammar = mom.grammar;
        child2->grammar = dad.grammar;

        // Invalidate the children
        child1->isPhenotypeValid = false;
        child2->isPhenotypeValid = false;
        child1->isEvaluated = false;
        child2->isEvaluated = false;

        // Add child1 and child2 to the children population
        children.individuals.push_back(child1);
        children.individuals.push_back(child2);

        // Increment the iterator by 2
        currentIndividualIt = currentIndividualIt + 2;
    }

    // Check to see if there is an individual left
    if (currentIndividualIt != parents.individuals.end())
    {
        // Add the last individual as fixed point would create a copy of the parent
        children.individuals.push_back(*currentIndividualIt);
    }

    return true;
}

// This method creates two children from two parents for fixed one point crossover
template <class POPULATIONTYPE>
bool GECrossover<POPULATIONTYPE>::fixedOnePointGenome(const GenomeType &mom, const GenomeType &dad, GenomeType &child1, GenomeType &child2)
{
    // Select a random number between 0 & 1
    std::uniform_real_distribution<> probability(0.0, 1.0);
    double result = probability(this->rng);

    // If the result is less than the crossover rate, perform crossover
    if (result < this->rate)
    {
        // Set the range to choose a crossover point
        std::uniform_int_distribution<> distribution(0, mom.genotype.size() < dad.genotype.size() ? mom.genotype.size() : dad.genotype.size());

        // Choose a crossover point
        unsigned int crossoverPoint = distribution(this->rng);

        // Copy codons to the children
        std::copy(mom.genotype.begin(), mom.genotype.begin() + crossoverPoint, std::back_inserter(child1.genotype));
        std::copy(dad.genotype.begin() + crossoverPoint, dad.genotype.end(), std::back_inserter(child1.genotype));
        std::copy(dad.genotype.begin(), dad.genotype.begin() + crossoverPoint, std::back_inserter(child2.genotype));
        std::copy(mom.genotype.begin() + crossoverPoint, mom.genotype.end(), std::back_inserter(child2.genotype));
    }
    else
    {
        // Copy parents to children directly
        // TODO: The parents should be added to the children population directly to avoid copying
        std::copy(mom.genotype.begin(), mom.genotype.end(), std::back_inserter(child1.genotype));
        std::copy(dad.genotype.begin(), dad.genotype.end(), std::back_inserter(child2.genotype));
    }
    return true;
}

#endif
