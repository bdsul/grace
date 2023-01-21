#ifndef _GENETICALGORITHM_HPP
#define _GENETICALGORITHM_HPP

// Include system libraries
#include <type_traits>
#include <climits>

// Include interfaces
#include "../interface/IEvolutionaryAlgorithm.hpp"
#include "../interface/IPopulation.hpp"
#include "../interface/IInitialiser.hpp"
#include "../interface/IMapper.hpp"
#include "../interface/IEvaluator.hpp"
#include "../interface/ISelection.hpp"
#include "../interface/ICrossover.hpp"
#include "../interface/IMutation.hpp"
#include "../interface/IReplacement.hpp"
#include "../interface/ITermination.hpp"
#include "../interface/IStatistics.hpp"

// This template class implements a basic Genetic Algorithm with the following structure

// Initial Generation:
//     1. Initialise the population
//     2. Map the population
//     3. Evaluate the population

// Subsequent Generations:
//     1. Select parents from the population
//     2. Create the children population using crossover
//     3. Mutate the children population
//     4. Map the children population
//     5. Evaluate the children population
//     6. Create a new population from the current and children population
//     7. Terminate if conditions are met, otherwise repeat

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
class GeneticAlgorithm : public IEvolutionaryAlgorithm
{
public:
    // Constructor/Destructor
    GeneticAlgorithm(int argc, char **argv, std::string settingsFile);
    ~GeneticAlgorithm();

    // Command Line & Settings file methods
    void addArguments(cxxopts::Options &);
    void parseArguments(cxxopts::ParseResult &);
    void parseSettings(INIReader &);

    // Genetic Algorithm methods
    void initialise();
    void step();
    void evolve();

public: // TODO: Should these be private and define access classes?
    // Check if operators are derived from the correct base classes
    static_assert(std::is_base_of<IPopulation, POPULATION>());
    static_assert(std::is_base_of<IInitialiser, INITIALISER>());
    static_assert(std::is_base_of<IMapper, MAPPER>());
    static_assert(std::is_base_of<IEvaluator, EVALUATOR>());
    static_assert(std::is_base_of<ICrossover, CROSSOVER>());
    static_assert(std::is_base_of<ISelection, SELECTION>());
    static_assert(std::is_base_of<IMutation, MUTATION>());
    static_assert(std::is_base_of<IReplacement, REPLACEMENT>());
    static_assert(std::is_base_of<ITermination, TERMINATION>());
    static_assert(std::is_base_of<IStatistics, STATISTICS>());

    // Single population of individuals
    POPULATION population;

    // Operator classes
    INITIALISER initialiser;
    MAPPER mapper;
    EVALUATOR evaluator;
    SELECTION selection;
    CROSSOVER crossover;
    MUTATION mutation;
    REPLACEMENT replacement;
    TERMINATION termination;
    STATISTICS statistics;

private:
    int rngSeed;
};

// Constructor
template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::GeneticAlgorithm(int argc, char **argv, std::string settingsFile)
    : rngSeed(0)
{
    // Initialise the command-line arguments
    cxxopts::Options arguments("GEGCC", "Grace - Written by Jack McEllin");

    // Add the help command line-argument
    arguments.add_options()("h,help", "Print usage");

    // Add operator command-line arguments
    this->addArguments(arguments);

    // Check if command-line argument help information is to be shown
    cxxopts::ParseResult result = arguments.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << arguments.help() << std::endl;
        exit(EXIT_SUCCESS);
    }

    // Check that the settings file is valid
    INIReader settings(settingsFile);
    if (settings.ParseError() < 0)
    {
        std::cout << "Error: Unable to load settings file. Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Read the settings file before reading the command line arguments
    this->parseSettings(settings);

    // Parse the rest of the command line arguments, overriding values set by the settings file
    this->parseArguments(result);

    // Initialise Algorithm's RNG
    rng.seed(rngSeed);

    // Initialise Operator RNG using values from the Algorithm's RNG
    initialiser.setRNGSeed(rng());
    mapper.setRNGSeed(rng());
    evaluator.setRNGSeed(rng());
    selection.setRNGSeed(rng());
    crossover.setRNGSeed(rng());
    mutation.setRNGSeed(rng());
    replacement.setRNGSeed(rng());
    termination.setRNGSeed(rng());
};

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::~GeneticAlgorithm(){
    // This destructor will call the destructors of the member classes automatically
};

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
void GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::addArguments(cxxopts::Options &options)
{
    // Add command-line arguments from the operator methods
    initialiser.addArguments(options);
    mapper.addArguments(options);
    evaluator.addArguments(options);
    selection.addArguments(options);
    crossover.addArguments(options);
    mutation.addArguments(options);
    replacement.addArguments(options);
    termination.addArguments(options);
    statistics.addArguments(options);
};

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
void GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::parseArguments(cxxopts::ParseResult &result)
{
    // Pass the command-line arguments to the operator methods
    initialiser.parseArguments(result);
    mapper.parseArguments(result);
    evaluator.parseArguments(result);
    selection.parseArguments(result);
    crossover.parseArguments(result);
    mutation.parseArguments(result);
    replacement.parseArguments(result);
    termination.parseArguments(result);
    statistics.parseArguments(result);
}

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
void GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::parseSettings(INIReader &settings)
{
    // Check if a RNG seed was given
    // This needs to be done before any other initialisation can happen
    if (settings.HasValue("GeneticAlgorithm", "RNGSeed"))
    {
        int rngSeed = settings.GetInteger("GeneticAlgorithm", "RNGSeed", 0);
        if (rngSeed < 0 || rngSeed > INT_MAX)
        {
            std::cout << "Error: RNG Seed range must be between 0 and INT_MAX. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Assign new seed
        this->rngSeed = rngSeed;

        // Reinitialise Algoritm's RNG
        rng.seed(rngSeed);

        // Reinitialise Operator RNG methods
        initialiser.setRNGSeed(rng());
        mapper.setRNGSeed(rng());
        evaluator.setRNGSeed(rng());
        selection.setRNGSeed(rng());
        crossover.setRNGSeed(rng());
        mutation.setRNGSeed(rng());
        replacement.setRNGSeed(rng());
        termination.setRNGSeed(rng());
    }

    // Pass settings file to each operator class for parsing
    initialiser.parseSettings(settings);
    mapper.parseSettings(settings);
    evaluator.parseSettings(settings);
    selection.parseSettings(settings);
    crossover.parseSettings(settings);
    mutation.parseSettings(settings);
    replacement.parseSettings(settings);
    termination.parseSettings(settings);
    statistics.parseSettings(settings);
}

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
void GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::initialise()
{
    // Initialise the initial population
    initialiser.initialise(population);

    // Map the initial population
    mapper.map(population);

    // Evaluate the initial population
    evaluator.evaluate(population);
};

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
void GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::step()
{
    // Select parents from the current population
    POPULATION parents;
    selection.select(population, parents);

    // Create the children from the parents
    POPULATION children;
    crossover.crossover(parents, children);

    // Mutate the children to add genetic diversity
    mutation.mutate(children);

    // Map the children
    mapper.map(children);

    // Evaluate the children
    evaluator.evaluate(children);

    // Replace the current population with the new population
    replacement.replace(population, children);
};

template <class POPULATION,
          class INITIALISER,
          class MAPPER,
          class EVALUATOR,
          class SELECTION,
          class CROSSOVER,
          class MUTATION,
          class REPLACEMENT,
          class TERMINATION,
          class STATISTICS>
void GeneticAlgorithm<POPULATION, INITIALISER, MAPPER, EVALUATOR, SELECTION, CROSSOVER, MUTATION, REPLACEMENT, TERMINATION, STATISTICS>::evolve()
{
    // Initialise Generation 0
    initialise();

    // Print Generation 0's statistics
    statistics.step(population);

    // Check for termination
    while (!termination.terminate(population)) // TODO: Add termination criteria
    {
        // Perform a generation
        step();

        // Print this generation's statistics
        statistics.step(population);
    }

    // Print final statistics
    statistics.end(population);
}

#endif
