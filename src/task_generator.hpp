#pragma once

#include <stdexcept>

#include "randoms.hpp"

namespace aco {

template <typename Task>
class TaskGenerator final {
 public:
  using Tasks = std::vector<Task>;

  TaskGenerator(Tasks tasks, Weights weights)
      : tasks_(std::move(tasks)),
        distribution_(RandomGenerator::GetInstance(), weights.begin(),
                      weights.end()) {
    if (tasks_.size() != weights.size()) {
      throw std::runtime_error(
          "The size of the weight vector does not match the number of targets");
    }
  }

  Task Get() {
    auto task_index = distribution_.Get();
    return tasks_[task_index];
  }

 private:
  Tasks tasks_;
  DiscreteDistribution distribution_;
};

std::vector<double> ReadWeights(std::string_view filepath);

using Target = uint32_t;
std::vector<Target> CreateTargets(size_t size);

}  // namespace aco