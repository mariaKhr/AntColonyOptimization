#include "randoms.hpp"

#include <iostream>

namespace aco {

RandomGenerator::RandomGenerator()
    : random_device_(), generator_(random_device_()) {}

std::mt19937 &RandomGenerator::Get() { return generator_; }

RandomGenerator &RandomGenerator::GetInstance() {
  static auto random_generator = RandomGenerator();
  return random_generator;
}

DiscreteDistribution::DiscreteDistribution(RandomGenerator &gen,
                                           Weights weights)
    : DiscreteDistribution(gen, weights.begin(), weights.end()) {}

int32_t DiscreteDistribution::Get() { return distribution_(gen_.Get()); }

}  // namespace aco