#include "randoms.hpp"

#include <iostream>

namespace aco {

RandomGenerator::RandomGenerator()
    : random_device_(), generator_(random_device_()) {}

std::mt19937& RandomGenerator::Get() { return generator_; }

DiscreteDistribution::DiscreteDistribution(RandomGenerator& gen,
                                           Weights weights)
    : DiscreteDistribution(gen, weights.begin(), weights.end()) {}

int32_t DiscreteDistribution::Get() { return distribution_(gen_.Get()); }

UniformDistribution::UniformDistribution(RandomGenerator& gen, double min,
                                         double max)
    : gen_(gen), distribution_(min, max) {}

double UniformDistribution::Get() { return distribution_(gen_.Get()); }

RandomGenerator& GetRandomGenerator() {
  static auto random_generator = RandomGenerator();
  return random_generator;
}

}  // namespace aco