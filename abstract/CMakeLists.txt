set(ABSTRACT_HEADERS
    "Arguments.hpp"
    "Settings.hpp"
    "RNG.hpp"
    "Genome.hpp"
    # "GEGenome.hpp"
    # "NSGA2Genome.hpp"
    "Population.hpp"
    "Initialiser.hpp"
    "Mapper.hpp"
    "Evaluator.hpp"
    "Selection.hpp"
    "Crossover.hpp"
    "Mutation.hpp"
    "Replacement.hpp"
    "Termination.hpp"
    "Statistics.hpp"
    "EvolutionaryAlgorithm.hpp"
    )

target_include_directories(${PROJECT_NAME} PRIVATE CMAKE_CURRENT_SOURCE_DIR)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${ABSTRACT_HEADERS} DESTINATION include/${PROJECT_NAME}/abstract)
    