#include <random>

namespace aco {

using Weight = double;
using Weights = std::initializer_list<Weight>;

class RandomGenerator final {
 public:
  RandomGenerator();
  std::mt19937& Get();

 private:
  std::random_device random_device_;
  std::mt19937 generator_;
};

class DiscreteDistribution final {
 public:
  explicit DiscreteDistribution(RandomGenerator& gen, Weights weights);

  template <typename Iter>
    requires std::forward_iterator<Iter>
  DiscreteDistribution(RandomGenerator& gen, Iter weights_begin, Iter weights_end)
      : gen_(gen),
        distribution_(weights_begin, weights_end) {}

  int32_t Get();

 private:
  RandomGenerator& gen_;
  std::discrete_distribution<> distribution_;
};

class UniformDistribution final {
 public:
  UniformDistribution(RandomGenerator& gen, double min, double max);

  double Get();

 private:
  RandomGenerator& gen_;
  std::uniform_real_distribution<> distribution_;
};

RandomGenerator& GetRandomGenerator();

}  // namespace aco