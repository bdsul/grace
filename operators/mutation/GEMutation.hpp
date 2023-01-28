#ifndef _GEMUTATION_HPP_
#define _GEMUTATION_HPP_

// Include abstract classes
#include "../../abstract/Mutation.hpp"

// This template class provides crossover methods that work
// with all classes that inherit GEGenome
template <class POPULATIONTYPE>
class GEMutation : public Mutation<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    GEMutation();           // Default constructor
    ~GEMutation() override; // Destructor

    // Command-line & settings file methods
    void addArguments(cxxopts::Options &) override;
    void parseArguments(cxxopts::ParseResult &) override;
    void parseSettings(INIReader &) override;

    // Implement pure virtual method from Mutation
    bool mutate(POPULATIONTYPE &population);

protected:
    // Available mutation methods
    bool codon(POPULATIONTYPE &population);

    // Variables
    float rate;

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (GEMutation::*MutationMethod)(POPULATIONTYPE &population);
    MutationMethod method;
};

// Default constructor
template <class POPULATIONTYPE>
GEMutation<POPULATIONTYPE>::GEMutation()
    : method(&GEMutation::codon),
      rate(0.05){};

// Destructor
template <class POPULATIONTYPE>
GEMutation<POPULATIONTYPE>::~GEMutation(){};

// Settings file parsing
template <class POPULATIONTYPE>
void GEMutation<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get mutation method
    if (settings.HasValue("GEMutation", "Method"))
    {
        std::string method = settings.Get("GEMutation", "Method", "UNKNOWN");

        if (method == "codon")
        {
            this->method = &GEMutation::codon;
        }
        else
        {
            std::cout << "Error: Invalid GEMutation method name. Exiting..." << std::endl;
            exit(1);
        }
    }

    // Get mutation rate
    if (settings.HasValue("GEMutation", "Rate"))
    {
        float rate = settings.GetReal("GEMutation", "Rate", -1.0);
        if (rate >= 0 && rate <= 1)
        {
            this->rate = rate;
        }
        else
        {
            std::cout << "Error: Invalid GEMutation rate. Exiting..." << std::endl;
            exit(1);
        }
    }
}

// Add command-line arguments
template <class POPULATIONTYPE>
void GEMutation<POPULATIONTYPE>::addArguments(cxxopts::Options &options)
{
    options.add_options("GEMutation")("m,mutationrate", "Mutation Rate", cxxopts::value<unsigned char>());
};

// Parse command-line arguments
template <class POPULATIONTYPE>
void GEMutation<POPULATIONTYPE>::parseArguments(cxxopts::ParseResult &results)
{
    if (results.count("mutationrate"))
    {
        this->rate = results["mutationrate"].as<float>();
        
        if (this->rate < 0.0 || this->rate > 1.0)
        {
            std::cout << "Error: Invalid GEMutation rate from command line. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool GEMutation<POPULATIONTYPE>::mutate(POPULATIONTYPE &population)
{
    return (this->*method)(population);
};

// Codon mutation
template <class POPULATIONTYPE>
bool GEMutation<POPULATIONTYPE>::codon(POPULATIONTYPE &population)
{
    // Set distribution ranges
    std::uniform_int_distribution<> codonDistibution(0, UINT8_MAX);
    std::uniform_real_distribution<> mutationProbability(0, 1);

    // For each individual in the population
    for (std::shared_ptr<GenomeType> &individual : population.individuals)
    {
        // For each codon in the individual
        for (unsigned int &codon : individual->genotype)
        {
            // Select a number between 0 & 1
            float result = mutationProbability(this->rng);

            // If the result is less than the mutation rate
            if (result < this->rate)
            {
                // Mutate the current codon
                codon = codonDistibution(this->rng);
            }

            // Otherwise, move onto the next codon
        }
    }
    return true;
}

#endif