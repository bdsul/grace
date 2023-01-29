#ifndef _GRACE_HPP_
#define _GRACE_HPP_

// Abstract classes
#include "./abstract/Arguments.hpp"
#include "./abstract/Crossover.hpp"
#include "./abstract/Evaluator.hpp"
#include "./abstract/EvolutionaryAlgorithm.hpp"
#include "./abstract/Genome.hpp"
#include "./abstract/Initialiser.hpp"
#include "./abstract/Mapper.hpp"
#include "./abstract/Mutation.hpp"
#include "./abstract/Population.hpp"
#include "./abstract/RNG.hpp"
#include "./abstract/Replacement.hpp"
#include "./abstract/Selection.hpp"
#include "./abstract/Settings.hpp"
#include "./abstract/Statistics.hpp"
#include "./abstract/Termination.hpp"

// Algorithms
#include "./algorithm/GeneticAlgorithm.hpp"

// Genomes
#include "./genome/GEGenome.hpp"
#include "./genome/FloatGenome.hpp"

// Populations
#include "./population/FloatPopulation.hpp"

// Operator classes
#include "./operators/crossover/GECrossover.hpp"
#include "./operators/initialiser/GEInitialiser.hpp"
#include "./operators/mapper/GEMapper.hpp"
#include "./operators/mutation/GEMutation.hpp"
#include "./operators/replacement/FloatReplacement.hpp"
#include "./operators/selection/FloatSelection.hpp"
#include "./operators/statistics/FloatStatistics.hpp"
#include "./operators/termination/GETermination.hpp"

// Utility classes
#include "util/DerivationTree.hpp"

#endif
