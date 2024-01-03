#pragma once

#include <random>

namespace aco {

using Weight = double;
using Weights = std::vector<Weight>;

class RandomGenerator final {
 public:
  static RandomGenerator &GetInstance();

  std::mt19937 &Get();

 private:
  RandomGenerator();

 private:
  std::random_device random_device_;
  std::mt19937 generator_;
};

class DiscreteDistribution final {
 public:
  DiscreteDistribution(RandomGenerator &gen, Weights weights);

  template <std::forward_iterator It>
  DiscreteDistribution(RandomGenerator &gen, It weights_begin, It weights_end)
      : gen_(gen), distribution_(weights_begin, weights_end) {}

  int32_t Get();

 private:
  RandomGenerator &gen_;
  std::discrete_distribution<> distribution_;
};

}  // namespace aco