#ifndef _GEINITIALISER_HPP_
#define _GEINITIALISER_HPP_

// Include abstract classes
#include "../../abstract/Initialiser.hpp"
#include "../../grammar/CFGrammar.hpp"

// This template class provides initialisation methods that work
// with all classes that inherit GEGenome
template <class POPULATIONTYPE>
class GEInitialiser : public Initialiser<POPULATIONTYPE>
{
public:
    // Define types to help readability
    using GenomeType = typename POPULATIONTYPE::GenomeType;
    using GenomePointer = typename POPULATIONTYPE::GenomePointer;
    using Individuals = typename POPULATIONTYPE::Individuals;

    // Constructor/Destructor
    GEInitialiser(); // Default constructor
    ~GEInitialiser() override; // Destructor

    // Command-line & settings file methods
    virtual void addArguments(cxxopts::Options &);
    virtual void parseArguments(cxxopts::ParseResult &);
    virtual void parseSettings(INIReader &);

    // Implement pure virtual method from Initialiser
    virtual bool initialise(POPULATIONTYPE &population) override;

protected:
    // Available initialisation methods
    bool random(POPULATIONTYPE &population);
    bool rvd(POPULATIONTYPE &population);
    bool sensible(POPULATIONTYPE &population);

    enum RHHTYPE
    {
        GROW,
        FULL
    };

    // Variables
    unsigned int genomeMinLength;
    unsigned int genomeMaxLength;
    unsigned int populationSize;
    // unsigned int sensibleMinDepth;
    unsigned int sensibleMaxDepth;
    CFGrammar grammarFile;

private:
    // Method pointer is private so that the prototype can be changed in the derived class
    // Note that this only hides the CrossoverMethod variable.
    // TODO: Template this so that the definition can change correctly.
    typedef bool (GEInitialiser::*InitialiserMethod)(POPULATIONTYPE &population);
    InitialiserMethod method;

    // Work methods
    bool createRandom(GenomeType &individual);
    bool createRampedHalfHalf(GenomeType &individual, RHHTYPE type);
    bool createTree(GenomeType &individual, int maxDepth, int type);
    bool createSubTree(GenomeType &individual, int maxDepth, int type, Symbol &currentSymbol, int currentDepth);
    bool createTail(GenomeType &individual);
};

// Default constructor
template <class POPULATIONTYPE>
GEInitialiser<POPULATIONTYPE>::GEInitialiser() : method(&GEInitialiser::random),
                                                 populationSize(100),
                                                 genomeMinLength(50),
                                                 genomeMaxLength(100),
                                                 sensibleMaxDepth(25){};

// Destructor
template <class POPULATIONTYPE>
GEInitialiser<POPULATIONTYPE>::~GEInitialiser(){};

// Settings file parsing
template <class POPULATIONTYPE>
void GEInitialiser<POPULATIONTYPE>::parseSettings(INIReader &settings)
{
    // Get initialisation method
    if (settings.HasValue("GEInitialiser", "Method"))
    {
        std::string method = settings.Get("GEInitialiser", "Method", "UNKNOWN");

        if (method == "random")
        {
            this->method = &GEInitialiser::random;
        }
        else if (method == "rvd")
        {
            this->method = &GEInitialiser::rvd;
        }
        else if (method == "sensible")
        {
            this->method = &GEInitialiser::sensible;
        }
        else
        {
            std::cout << "Error: Invalid GEInitialiser method name. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Get grammar file
    if (settings.HasValue("GEInitialiser", "GrammarFile"))
    {
        std::string grammarFile = settings.Get("GEInitialiser", "GrammarFile", "UNKNOWN");

        if (grammarFile == "UNKNOWN")
        {
            std::cout << "Error: Invalid GEInitialiser grammar filename. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
        this->grammarFile.readBNFFile(grammarFile);
    }

    // Get initial population size
    if (settings.HasValue("GEInitialiser", "PopulationSize"))
    {
        int populationSize = settings.GetInteger("GEInitialiser", "PopulationSize", -1);

        if (populationSize < 0 || populationSize > INT_MAX)
        {
            std::cout << "Warning: Invalid GEInitialiser populationSize. Using default of 100..." << std::endl;
            populationSize = 100;
        }
        this->populationSize = populationSize;
    }

    // Get initial population's minimum genome length
    if (settings.HasValue("GEInitialiser", "MinGenomeLength"))
    {
        int genomeMinLength = settings.GetInteger("GEInitialiser", "MinGenomeLength", -1);

        if (genomeMinLength < 0 || genomeMinLength > INT_MAX)
        {
            std::cout << "Error: Invalid GEInitialiser minimum genome length. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
        this->genomeMinLength = genomeMinLength;
    }

    // Get initial population's maximum genome length
    if (settings.HasValue("GEInitialiser", "MaxGenomeLength"))
    {
        int genomeMaxLength = settings.GetInteger("GEInitialiser", "MaxGenomeLength", -1);

        if (genomeMaxLength < 0 || genomeMaxLength > INT_MAX)
        {
            std::cout << "Error: Invalid GEInitialiser maximum genome length. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
        this->genomeMaxLength = genomeMaxLength;
    }

    if (settings.HasValue("GEInitialiser", "SensibleMaxDepth"))
    {
        int sensibleMaxDepth = settings.GetInteger("GEInitialiser", "SensibleMaxDepth", -1);

        if (sensibleMaxDepth < 0 || sensibleMaxDepth > INT_MAX)
        {
            std::cout << "Error: Invalid GEInitialiser sensible max depth. Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }

        this->sensibleMaxDepth = sensibleMaxDepth;
    }
};

// Add command-line arguments
template <class POPULATIONTYPE>
void GEInitialiser<POPULATIONTYPE>::addArguments(cxxopts::Options &options)
{
    options.add_options("GEInitialiser")("p,population", "Population Size", cxxopts::value<unsigned char>());
    options.add_options("GEInitialiser")("b,bnfgrammar", "Grammar File", cxxopts::value<std::string>());
};

// Parse command-line arguments
template <class POPULATIONTYPE>
void GEInitialiser<POPULATIONTYPE>::parseArguments(cxxopts::ParseResult &results)
{
    // TODO: Add exception handling
    if (results.count("population"))
    {
        this->populationSize = results["population"].as<unsigned char>();
    }
    if (results.count("bnfgrammar"))
    {
        this->grammarFile.readBNFFile(results["bnfgrammar"].as<std::string>());
    }
};

// Implement pure virtual method from base class
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::initialise(POPULATIONTYPE &population)
{
    return (this->*method)(population);
};

// Random initialisation
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::random(POPULATIONTYPE &population)
{
    for (unsigned int i = 0; i < this->populationSize; ++i)
    {
        // Add new individual to population
        GenomePointer individual = std::make_shared<GenomeType>();

        // Initialise the individual
        createRandom(*individual);

        // Add the grammar to the individual
        individual->grammar = this->grammarFile;

        // Add generated individual to the initial population
        population.individuals.push_back(individual);
    }

    return true;
}

// Random initialisation with valids and no duplicates (RVD)
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::rvd(POPULATIONTYPE &population)
{
    for (unsigned int i = 0; i < this->populationSize; ++i)
    {
        // Add new individual to population
        GenomePointer individual = std::make_shared<GenomeType>();

        // Initialise the individual
        createRandom(*individual);

        // Check if the individual is a duplicate
        bool isDuplicate = false;
        do
        {
            isDuplicate = false;

            for (GenomePointer &member : population.individuals)
            {
                if (individual == member)
                {
                    isDuplicate = true;
                }
            }
        } while (isDuplicate == true);

        // Add the grammar to the individual
        individual->grammar = this->grammarFile;

        // Add generated unique individual to the initial population
        population.individuals.push_back(individual);
    }

    return true;
}

// Sensible initialisation (Ramped Half & Half)
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::sensible(POPULATIONTYPE &population)
{
    // Check that population size is greater than 2
    if (this->populationSize < 2)
    {
        std::cout << "Error: Population size is too small for Ramped Half Half initialisation. Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Check for even population
    if (this->populationSize % 2 == 1)
    {
        std::cout << "Warning: Population size is odd. Adding one extra individual. Continuing..." << std::endl;

        // Add an extra individual to make the population even
        ++this->populationSize;
    }

    // Get minimum depth for start symbol
    unsigned int minimumDepth = grammarFile.getStartRule()->getMinimumDepth();

    // Get range of depths - Min and Max inbetween
    if (minimumDepth > sensibleMaxDepth)
    {
        std::cout << "Error: Minimum rhh depth is greater than Maximum." << std::endl;
        std::cout << "Error: Minimum rhh depth: " << minimumDepth << std::endl;
        std::cout << "Error: Maximum rhh depth: " << this->sensibleMaxDepth << std::endl;
        std::cout << "Error: Exiting..." << std::endl;

        exit(EXIT_FAILURE);
    }

    // Create our range of depths
    std::vector<unsigned int> depths;

    for (int depth = minimumDepth; depth <= sensibleMaxDepth; depth++)
    {
        depths.push_back(depth);
    }

    // 5. Check if we can cover all the ramping depths
    if ((this->populationSize / 2) < depths.size())
    {
        std::cout << "Error: Population size is too small to cover all depths. Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // 6. Split by N depths
    // TODO: Give these variables a better name
    unsigned int quotient = (this->populationSize / depths.size()) / 2;
    unsigned int remainder = this->populationSize - (quotient * depths.size() * 2);

    // Loop over the population
    for (int q = 0; q < quotient; ++q)
    {
        for (unsigned int depth : depths)
        {
            // Generate a GROW and FULL individual for each depth
            // TODO: Add grow rate here. Currently set to 50:50 between GROW and FULL
            for (int i = 0; i < 2; ++i)
            {
                // Create individual
                GenomePointer individual = std::make_shared<GenomeType>();

                // Add the grammar to the individual
                individual->grammar = this->grammarFile;

                // Initialise using GROW or FULL method
                if (i == 0)
                {
                    createTree(*individual, depth, GROW);
                }
                else
                {
                    createTree(*individual, depth, GROW);
                }

                // Add tails
                createTail(*individual);

                // Add the initialised individual to the population
                population.individuals.push_back(individual);
            }
        }
    }

    // Create remaining individuals - Select from valid depths
    std::uniform_int_distribution<> depthDistribution(0, depths.size() - 1);

    for (int i = 0; i < remainder; ++i)
    {
        // Select a random depth
        unsigned int depth = depths.at(depthDistribution(this->rng));

        // Create individual
        GenomePointer individual = std::make_shared<GenomeType>();

        // Add the grammar to the individual
        individual->grammar = this->grammarFile;

        // Initialise using GROW or FULL method
        // TODO: Add grow rate here, currently set to 50:50
        if (i % 2 == 0)
        {
            createTree(*individual, depth, GROW);
        }
        else
        {
            createTree(*individual, depth, GROW);
        }

        // Add tails
        // TODO: Add a setting to enable/disable this
        createTail(*individual);

        // Add the initialised individual to the population
        population.individuals.push_back(individual);
    }

    return 0;
}

// Create an individual with a random genome length
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::createRandom(GenomeType &individual)
{
    // Select our range to generate codons in (0-255)
    std::uniform_int_distribution<> codonDistribution(0, UINT8_MAX);
    std::uniform_int_distribution<> genomeLengthDistribution(this->genomeMinLength, this->genomeMaxLength);

    // Choose a random length
    unsigned int genomeLength = genomeLengthDistribution(this->rng);

    // Initialise genotype
    for (unsigned int j = 0; j < genomeLength; ++j)
    {
        // TODO: Maybe replace this rand function?
        individual.genotype.push_back(codonDistribution(this->rng));
    }

    return true;
}

// Top level calling method to perform sensible initialisation
// Creates a derivation tree, hence the name
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::createTree(GenomeType &individual, int maxDepth, int type)
{
    // Call recursive function with the start symbol
    // Derivation tree starts at a depth of 0 for the root node
    return createSubTree(individual, maxDepth, type, *individual.grammar.getStartSymbol(), 0);
}

// Generate codons for the selected level of the derivation tree
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::createSubTree(GenomeType &individual, int maxDepth, int type, Symbol &currentSymbol, int currentDepth)
{
    // Is the current depth greater than the maximum depth?
    if (currentDepth > maxDepth)
    {
        return false;
    }

    // If the symbol is a terminal, return
    if (currentSymbol.getType() == Symbol::TerminalSymbol)
    {
        return true;
    }

    // Symbol is a non-terminal. Get the corrosponding rule
    const CFRule *currentRule = individual.grammar.findRule(currentSymbol);

    // Was the rule found?
    if (currentRule == nullptr)
    {
        std::cout << "Error: Unable to find Rule for Symbol " << currentSymbol.getValue() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Get the possible choices
    std::vector<unsigned int> validChoicesIndex;

    for (int index = 0; index < currentRule->rhs.size(); ++index)
    {
        // Don't exceed the maximum depth
        if (currentDepth + currentRule->rhs.at(index).getMinimumDepth() > maxDepth)
        {
            continue;
        }

        // Check if we're at the max depth
        if (currentDepth == maxDepth)
        {
            // We can only return choices that exclusively contain terminals
            bool isValid = true;

            // Does the choice contain a non-terminal?
            for (std::shared_ptr<Symbol> symbol : currentRule->rhs.at(index).symbols)
            {
                if (symbol->getType() == Symbol::NonTerminalSymbol)
                {
                    isValid = false;
                    break;
                }
            }

            // Is the choice valid?
            if (isValid == false)
            {
                continue;
            }

            // Add the choice to our possible choices
            validChoicesIndex.push_back(index);
        }

        // For FULL, we have to have at least one non-terminal
        // We also need to check that the choice can map in the desired depth
        // Although I don't know if the above check removes the need for this
        if (type == FULL)
        {
            // We can only return choices that contain nonterminals
            // They also need to be mappable in at the desired depth
            bool isValid = false;
            for (std::shared_ptr<Symbol> symbol : currentRule->rhs.at(index).symbols)
            {
                if (symbol->getType() == Symbol::NonTerminalSymbol)
                {
                    isValid = true;
                    break;
                }
            }

            // If the choice contains a non-terminal, add it to our possible choices
            if (isValid == true)
            {
                validChoicesIndex.push_back(index);
            }
        }
        else
        {
            // Grow method - We can choose either
            validChoicesIndex.push_back(index);
        }
    }

    // Is there a choice to choose?
    if (validChoicesIndex.empty())
    {
        std::cout << "Error: No Choices possible" << std::endl;
        return false;
    }

    unsigned int chosenChoiceIndex = 0;

    if (currentRule->rhs.size() == 1)
    {
        // If there's only one choice, we don't need a codon
    }
    else
    {
        unsigned int codon;
        std::uniform_int_distribution<> codonDistribution(0, (UINT8_MAX / currentRule->rhs.size()));

        // If there's multiple choices, we need a codon
        if (validChoicesIndex.size() > 1)
        {
            // We need to choose a possible choice
            std::uniform_int_distribution<> choiceDistribution(0, validChoicesIndex.size() - 1);
            chosenChoiceIndex = validChoicesIndex.at(choiceDistribution(this->rng));
        }
        else
        {
            // Add the only valid choice.
            // A codon will still be required as there is more than one choice,
            // even if some choices are invalid
            chosenChoiceIndex = validChoicesIndex.at(0);
        }

        // Add the codon to the genome
        codon = (codonDistribution(this->rng) * currentRule->rhs.size()) + chosenChoiceIndex;
        individual.genotype.push_back(codon);
    }

    // For each symbol in the chosen production, recursively call this function
    for (std::shared_ptr<Symbol> symbol : currentRule->rhs.at(chosenChoiceIndex).symbols)
    {
        bool isValid = createSubTree(individual, maxDepth, type, *symbol, currentDepth + 1);
        if (isValid == false)
        {
            return false;
        }
    }

    return true;
}

// Add codons to the end of an individual
template <class POPULATIONTYPE>
bool GEInitialiser<POPULATIONTYPE>::createTail(GenomeType &individual)
{
    // Select our range to generate codons in (0-255)
    // TODO: Make whether tails are added or not clearer. (Negative won't add, make this explicit)
    std::uniform_int_distribution<> codonDistribution(0, UINT8_MAX);

    // Make sure the length is in the correct range
    std::uniform_int_distribution<> genomeLengthDistribution(this->genomeMinLength - individual.genotype.size(), this->genomeMaxLength - individual.genotype.size());

    // Choose a random length
    unsigned int genomeLength = genomeLengthDistribution(this->rng);

    // Initialise genotype
    for (unsigned int j = 0; j < genomeLength; ++j)
    {
        individual.genotype.push_back(codonDistribution(this->rng));
    }

    return true;
}

#endif
