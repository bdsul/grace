#ifndef _FLOATSELECTION_HPP_
#define _FLOATSELECTION_HPP_

// Include system libraries
#include <random>

// Include abstract classes
#include "../../abstract/Selection.hpp"
#include "../../population/FloatPopulation.hpp"

// This template class provides selection methods that work
// with all classes that inherit GEGenome
template <class POPULATIONTYPE = FloatPopulation>
class FloatSelection : public Selection<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    FloatSelection();           // Default constructor
    ~FloatSelection() override; // Destructor

    // Settings file methods
    void parseSettings(INIReader &) override;

    bool select(POPULATIONTYPE &population, POPULATIONTYPE &parents) override;

protected:
    // Available selection methods
    bool rouletteWheelSelection(POPULATIONTYPE &population, POPULATIONTYPE &parents);

    // Variable
    bool replacementEnabled;

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (FloatSelection::*SelectionMethod)(POPULATIONTYPE &population, POPULATIONTYPE &parents);
    SelectionMethod method;
};

// Default constructor
template <class POPULATIONTYPE>
FloatSelection<POPULATIONTYPE>::FloatSelection()
    : method(&FloatSelection::rouletteWheelSelection),
      replacementEnabled(true){};

// Destructor
template <class POPULATIONTYPE>
FloatSelection<POPULATIONTYPE>::~FloatSelection(){};

// Settings file parsing
template <class POPULATIONTYPE>
void FloatSelection<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get selection method
    if (settings.HasValue("FloatSelection", "Method"))
    {
        std::string method = settings.Get("FloatSelection", "Method", "UNKNOWN");

        if (method == "roulette")
        {
            this->method = &FloatSelection::rouletteWheelSelection;
        }
        else
        {
            std::cout << "Error: Invalid FloatSelection method name. Exiting..." << std::endl;
            exit(1);
        }
    }

    // Get individual replacement flag
    // TODO: Add exception handling here
    if (settings.HasValue("FloatSelection", "Replacement"))
    {
        bool replacementEnabled = settings.GetBoolean("FloatSelection", "Replacement", false);
        this->replacementEnabled = replacementEnabled;
    }
}

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool FloatSelection<POPULATIONTYPE>::select(POPULATIONTYPE &population, POPULATIONTYPE &parents)
{
    return (this->*method)(population, parents);
};

// Roulette wheel selection
template <class POPULATIONTYPE>
bool FloatSelection<POPULATIONTYPE>::rouletteWheelSelection(POPULATIONTYPE &population, POPULATIONTYPE &parents)
{
    POPULATIONTYPE candidates;

    // Get valid candidates
    for (std::shared_ptr<GenomeType> individual : population.individuals)
    {
        if (individual->isPhenotypeValid == true)
        {
            candidates.individuals.push_back(individual);
        }
    }

    // Check that we have enough candidates to proceed
    if (replacementEnabled)
    {
        if (candidates.individuals.size() == 0)
        {
            std::cout << "Error: Zero valid candidates for selection with replacement. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (candidates.individuals.size() < population.individuals.size())
        {
            std::cout << "Error: Not enough candidates to fill population without replacement. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Get sum of scores
    double sumOfScores = 0;
    for (std::shared_ptr<GenomeType> individual : candidates.individuals)
    {
        sumOfScores += individual->score;
    }

    // Create a distribution to choose a random number in range
    std::uniform_real_distribution<> choice(0, sumOfScores);

    // Repeat until we've chosen enough parents
    while (parents.individuals.size() < population.individuals.size())
    {
        // Select a number in the range of 0 to sumOfScores
        double selection = choice(this->rng);

        // Find the corresponding individual
        double tempSum = 0;
        typename Individuals::iterator individualIt = candidates.individuals.begin();

        // Select the individual
        while (tempSum <= selection)
        {
            // Add score
            tempSum += individualIt->get()->score;

            // Does the selected number lie in the current individual?
            if (tempSum >= selection)
            {
                break;
            }
            else
            {
                ++individualIt;
            }
        }

        // Add the selected individual to the parents
        parents.individuals.push_back(*individualIt);

        // If replacement is disabled, remove the individual from the candidates
        if (!replacementEnabled)
        {
            candidates.individuals.erase(individualIt);
        }
    }

    return true;
}

#endif
