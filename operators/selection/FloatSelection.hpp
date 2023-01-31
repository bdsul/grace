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
    bool isMinimizationProblem;

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

    // Get maximization/minimization problem
    if (settings.HasValue("FloatSelection", "ProblemType"))
    {
        std::string problemType = settings.Get("FloatSelection", "ProblemType", "UNKNOWN");

        if (problemType == "Maximization")
        {
            this->isMinimizationProblem = false;
        }
        else if (problemType == "Minimization")
        {
            this->isMinimizationProblem = true;
        }
        else
        {
            std::cout << "Error: Invalid FloatSelection problem type. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
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

    // Get sum of individuals
    double sumOfScores = 0;
    for (std::shared_ptr<GenomeType> individual : candidates.individuals)
    {
        sumOfScores += individual->score;
    }

    // Get the normalised probabilities of each individual being chosen
    std::vector<float> candidateProbabilities;
    for (std::shared_ptr<GenomeType> individual : candidates.individuals)
    {
        float probability = 0.0;

        // Check the problem type
        if (isMinimizationProblem)
        {
            // Subtract normal probability from 1, and divide by number of individuals minus 1 to normalise again
            probability = (1.0 - (individual->score / sumOfScores)) / (candidates.individuals.size() - 1);
        }
        else
        {
            // Normal probability, also used in the formula above
            probability = individual->score / sumOfScores;
        }

        // Add calculated probability to probabilities vector
        candidateProbabilities.push_back(probability);
    }

    // Scores will be normalised before selection
    std::uniform_real_distribution<> choice(0.0, 1.0);

    // Repeat until we've chosen enough parents
    while (parents.individuals.size() < population.individuals.size())
    {
        // Select a number in the range of 0 to 1
        double selection = choice(this->rng);

        // Find the corresponding individual
        double currentSumOfProbabilities = 0;
        typename Individuals::iterator individualIt = candidates.individuals.begin();
        std::vector<float>::iterator probabilityIt = candidateProbabilities.begin();

        // Select the individual
        while (currentSumOfProbabilities <= selection)
        {
            // Add the current individual's probabilty to the current sum
            currentSumOfProbabilities += *probabilityIt;

            // Check if the sum exceeds the chosen random number
            // If so, individualIt points to the chosen individual
            if (currentSumOfProbabilities >= selection)
            {
                break;
            }
            else
            {
                ++individualIt;
                ++probabilityIt;
            }
        }

        // Add the selected individual to the parents
        parents.individuals.push_back(*individualIt);

        // If replacement is disabled, remove the individual from the candidates
        if (!replacementEnabled)
        {
            // Erase the individual
            candidates.individuals.erase(individualIt);

            // Normalise the other probabilites so we can remove the current individual
            for (auto probability : candidateProbabilities)
            {
                probability = probability / (1.0 - *probabilityIt);
            }

            // Erase the current individual's probability
            candidateProbabilities.erase(probabilityIt);
        }
    }

    return true;
}

#endif
