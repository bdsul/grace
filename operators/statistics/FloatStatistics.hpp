#ifndef _FLOATSTATISTICS_HPP_
#define _FLOATSTATISTICS_HPP_

// Include system libraries
#include <numeric>

// Include abstract classes
#include "../../abstract/Statistics.hpp"
#include "../../population/FloatPopulation.hpp"

// This template class provides statistic methods that work
// with all classes that inherit FloatGenome
template <class POPULATIONTYPE = FloatPopulation>
class FloatStatistics : public Statistics<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    FloatStatistics();           // Default constructor
    ~FloatStatistics() override; // Destructor

    // Implement pure virtual methods from Statistics
    bool step(POPULATIONTYPE &population) override;
    bool end(POPULATIONTYPE &population) override;

protected:
    // Available crossover methods
    bool floatStep(POPULATIONTYPE &population);
    bool floatEnd(POPULATIONTYPE &population);

    // Variables
    int currentGeneration;

    // Work methods
    float getMean(POPULATIONTYPE &population);
    float getMax(POPULATIONTYPE &population);
    float getMin(POPULATIONTYPE &population);
    float getStandardDeviation(POPULATIONTYPE &population);
    void printBestIndividual(POPULATIONTYPE &population);

private:
     // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (FloatStatistics::*StepMethod)(POPULATIONTYPE &population);
    typedef bool (FloatStatistics::*EndMethod)(POPULATIONTYPE &population);

    StepMethod stepFunction;
    EndMethod endFunction;
};

// Default constructor
template <class POPULATIONTYPE>
FloatStatistics<POPULATIONTYPE>::FloatStatistics()
    : stepFunction(&FloatStatistics::floatStep),
      endFunction(&FloatStatistics::floatEnd),
      currentGeneration(0){};

// Destructor
template <class POPULATIONTYPE>
FloatStatistics<POPULATIONTYPE>::~FloatStatistics(){};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool FloatStatistics<POPULATIONTYPE>::step(POPULATIONTYPE &population)
{
    return (this->*stepFunction)(population);
};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool FloatStatistics<POPULATIONTYPE>::end(POPULATIONTYPE &population)
{
    return (this->*endFunction)(population);
};

// Step method for FloatGenome
template <class POPULATIONTYPE>
bool FloatStatistics<POPULATIONTYPE>::floatStep(POPULATIONTYPE &population)
{
    // TODO: Print out the statistics into a log file
    float mean = getMean(population);
    float max = getMax(population);
    float min = getMin(population);
    float sd = getStandardDeviation(population);

    // Output to screen
    std::cout << currentGeneration << '\t' << mean << '\t' << max << '\t' << min << '\t' << sd << std::endl;

    // Increment current generation
    ++currentGeneration;

    return true;
}

// End method for FloatGenome
template <class POPULATIONTYPE>
bool FloatStatistics<POPULATIONTYPE>::floatEnd(POPULATIONTYPE &population)
{
    printBestIndividual(population);

    return true;
}

template <class POPULATIONTYPE>
float FloatStatistics<POPULATIONTYPE>::getMean(POPULATIONTYPE &population)
{
    float mean = 0;

    // Add up the scores of the population
    for (std::shared_ptr<FloatGenome> &individual : population.individuals)
    {
        mean += individual->score;
    }

    // Divide by the population size
    mean = mean / population.individuals.size();

    return mean;
}

template <class POPULATIONTYPE>
float FloatStatistics<POPULATIONTYPE>::getMax(POPULATIONTYPE &population)
{
    float max = 0;

    // Use iterator to access elements
    auto it = population.individuals.begin();

    // Set first element to compare the rest against
    if (it != population.individuals.end())
    {
        max = it->get()->score;
        ++it;
    }

    // Check the rest of the elements
    while (it != population.individuals.end())
    {
        if (max < it->get()->score)
        {
            max = it->get()->score;
        }
        ++it;
    }

    // Return the largest element
    return max;
}

template <class POPULATIONTYPE>
float FloatStatistics<POPULATIONTYPE>::getMin(POPULATIONTYPE &population)
{
    float min = 0;

    // Use iterator to access elements
    auto it = population.individuals.begin();

    // Set first element to compare the rest against
    if (it != population.individuals.end())
    {
        min = it->get()->score;
        ++it;
    }

    // Check the rest of the elements
    while (it != population.individuals.end())
    {
        if (min > it->get()->score)
        {
            min = it->get()->score;
        }
        ++it;
    }

    // Return the smallest element
    return min;
}

template <class POPULATIONTYPE>
float FloatStatistics<POPULATIONTYPE>::getStandardDeviation(POPULATIONTYPE &population)
{
    float sum = 0;

    // Add up the scores of the population
    for (std::shared_ptr<FloatGenome> &individual : population.individuals)
    {
        sum += individual->score;
    }

    // Get X Hat (average value)
    float xhat = sum / population.individuals.size();

    // Get summation of sum((X-XHat)^2)
    float summation = 0;
    for (std::shared_ptr<FloatGenome> &individual : population.individuals)
    {
        summation += pow((individual->score - xhat), 2);
    }

    // Get standard deviation by dividing by n-1
    float sd = sqrt(summation / (population.individuals.size() - 1));

    // Return the standard deviation
    return sd;
}

template <class POPULATIONTYPE>
void FloatStatistics<POPULATIONTYPE>::printBestIndividual(POPULATIONTYPE &population)
{
    float max = 0;

    // Use iterator to access elements
    auto it = population.individuals.begin();
    auto best = it;

    // Set first element to compare the rest against
    if (it != population.individuals.end())
    {
        max = it->get()->score;
        ++it;
    }

    // Check the rest of the elements
    while (it != population.individuals.end())
    {
        if (max < it->get()->score)
        {
            max = it->get()->score;
            best = it;
        }
        ++it;
    }

    // Print best individual
    std::cout << "Best Individual: " << std::endl;
    std::cout << "Genotype: " << std::endl;
    for (auto codon : best->get()->genotype)
    {
        std::cout << codon << " ";
    }
    std::cout << std::endl;
    std::cout << "Phenotype: " << best->get()->phenotype << std::endl;
    std::cout << "Score: " << best->get()->score << std::endl;

}

#endif
