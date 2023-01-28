#ifndef _FLOATREPLACEMENT_HPP_
#define _FLOATREPLACEMENT_HPP_

// Include abstract classes
#include "../../abstract/Replacement.hpp"
#include "../../population/FloatPopulation.hpp"

// This template class provides replacement methods that work
// with all classes that inherit FloatGenome
template <class POPULATIONTYPE = FloatPopulation>
class FloatReplacement : public Replacement<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    FloatReplacement();           // Default constructor
    ~FloatReplacement() override; // Destructor

    // Settings file method
    void parseSettings(INIReader &) override;

    // Implement pure virtual method from Replacement
    bool replace(POPULATIONTYPE &population, POPULATIONTYPE &children) override;

protected:
    // Available replacement methods
    bool generational(POPULATIONTYPE &population, POPULATIONTYPE &children);

    // Variables
    float elitismRate;

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (FloatReplacement::*ReplacementMethod)(POPULATIONTYPE &population, POPULATIONTYPE &children);
    ReplacementMethod method;
};

// Default constructor
template <class POPULATIONTYPE>
FloatReplacement<POPULATIONTYPE>::FloatReplacement()
    : method(&FloatReplacement::generational),
      elitismRate(0.1){};

// Destructor
template <class POPULATIONTYPE>
FloatReplacement<POPULATIONTYPE>::~FloatReplacement(){};

// Settings file parsing
template <class POPULATIONTYPE>
void FloatReplacement<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get replacement method
    if (settings.HasValue("FloatReplacement", "Method"))
    {
        std::string method = settings.Get("FloatReplacement", "Method", "UNKNOWN");

        if (method == "generational")
        {
            this->method = &FloatReplacement::generational;
        }
        else
        {
            std::cout << "Error: Invalid FloatReplacement method name. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Get elitism rate
    if (settings.HasValue("FloatReplacement", "ElitismRate"))
    {
        float elitismRate = settings.GetReal("FloatReplacement", "ElitismRate", -1.0);
        if (elitismRate >= 0 && elitismRate <= 1)
        {
            this->elitismRate = elitismRate;
        }
        else
        {
            std::cout << "Error: Invalid FloatReplacement elitism rate. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool FloatReplacement<POPULATIONTYPE>::replace(POPULATIONTYPE &population, POPULATIONTYPE &children)
{
    return (this->*method)(population, children);
};

// Generational replacement - Add best elites and children
template <class POPULATIONTYPE>
bool FloatReplacement<POPULATIONTYPE>::generational(POPULATIONTYPE &population, POPULATIONTYPE &children)
{
    POPULATIONTYPE newPopulation;

    // Sort both populations, Max score first
    std::sort(std::begin(population.individuals), std::end(population.individuals), [](std::shared_ptr<GenomeType> a, std::shared_ptr<GenomeType> b)
              { return a->score > b->score; });
    std::sort(std::begin(children.individuals), std::end(children.individuals), [](std::shared_ptr<GenomeType> a, std::shared_ptr<GenomeType> b)
              { return a->score > b->score; });

    // Make sure our new population is the correct size
    newPopulation.individuals.resize(population.individuals.size());

    // Get number of individuals to keep with elitism
    int elitismSize = floor(population.individuals.size() * elitismRate);

    // Add the elites
    for (int i = 0; i < elitismSize; ++i)
    {
        newPopulation.individuals.at(i) = population.individuals.at(i);
    }

    // Add the children
    for (int i = elitismSize; i < population.individuals.size(); ++i)
    {
        newPopulation.individuals.at(i) = children.individuals.at(i - elitismSize);
    }

    // Set the population to the new population
    population = newPopulation;

    return true;
}

#endif
