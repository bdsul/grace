#ifndef _GETERMINATION_HPP_
#define _GETERMINATION_HPP_

// Include abstract classes
#include "../../abstract/Termination.hpp"

// This template class provides termination methods that work
// with all classes that inherit GEGenome
template <class POPULATIONTYPE>
class GETermination : public Termination<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    GETermination();           // Default constructor
    ~GETermination() override; // Destructor

    // Command-line & settings file methods
    void addArguments(cxxopts::Options &) override;
    void parseArguments(cxxopts::ParseResult &) override;
    void parseSettings(INIReader &) override;

    // Implement pure virtual method from Termination
    bool terminate(POPULATIONTYPE &population) override;

protected:
    // Available termination methods
    bool maxGenerationTermination(POPULATIONTYPE &population);

    // Variables
    unsigned int maxGenerations;
    unsigned int currentGeneration;

    // Work methods
    bool isMaxGenerations();

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (GETermination<POPULATIONTYPE>::*TerminationMethod)(POPULATIONTYPE &population);
    TerminationMethod method;
};

// Default constructor
template <class POPULATIONTYPE>
GETermination<POPULATIONTYPE>::GETermination() : method(&GETermination<POPULATIONTYPE>::maxGenerationTermination),
                                                 maxGenerations(10),
                                                 currentGeneration(0){};

// Destructor
template <class POPULATIONTYPE>
GETermination<POPULATIONTYPE>::~GETermination(){};

// Settings file parsing
template <class POPULATIONTYPE>
void GETermination<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get termination method
    if (settings.HasValue("GETermination", "Method"))
    {
        std::string method = settings.Get("GETermination", "Method", "UNKNOWN");

        if (method == "maxGenerationTermination")
        {
            this->method = &GETermination<POPULATIONTYPE>::maxGenerationTermination;
        }
        else
        {
            std::cout << "Error: Invalid termination method name. Exiting..." << std::endl;
            exit(1);
        }
    }

    // Get termination max generations
    if (settings.HasValue("GETermination", "MaxGenerations"))
    {
        int maxGenerations = settings.GetReal("GETermination", "MaxGenerations", -1);
        if (maxGenerations >= 0 && maxGenerations < INT_MAX)
        {
            this->maxGenerations = maxGenerations;
        }
        else
        {
            std::cout << "Error: Invalid termination max generations. Exiting..." << std::endl;
            exit(1);
        }
    }
}

// Add command-line arguments
template <class POPULATIONTYPE>
void GETermination<POPULATIONTYPE>::addArguments(cxxopts::Options &options)
{
    options.add_options("GETermination")("g,maxgenerations", "Max Generations", cxxopts::value<int>());
};

// Parse command-line arguments
template <class POPULATIONTYPE>
void GETermination<POPULATIONTYPE>::parseArguments(cxxopts::ParseResult &results)
{
    // TODO: Add exception handling
    if (results.count("maxgenerations"))
    {
        this->maxGenerations = results["maxgenerations"].as<int>();
    }
};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool GETermination<POPULATIONTYPE>::terminate(POPULATIONTYPE &population)
{
    return (this->*method)(population);
}

// Max generation only method that can be assigned to the TerminationMethod pointer
template <class POPULATIONTYPE>
bool GETermination<POPULATIONTYPE>::maxGenerationTermination(POPULATIONTYPE &population)
{
    return isMaxGenerations();
}

// Work method that checks if the current generation is greater than the max generations
template <class POPULATIONTYPE>
bool GETermination<POPULATIONTYPE>::isMaxGenerations()
{
    // If we've exceeded the number of generations, terminate
    if (this->currentGeneration >= this->maxGenerations)
    {
        return true;
    }

    // Otherwisem , increment the current generation
    this->currentGeneration = this->currentGeneration + 1;

    return false;
}

#endif
