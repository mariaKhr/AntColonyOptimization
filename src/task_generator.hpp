#pragma once

#include "randoms.hpp"

namespace aco {

template <typename Task>
class TaskGenerator final {
 public:
  using Tasks = std::vector<Task>;

  TaskGenerator(Tasks tasks, Weights weights)
      : tasks_(std::move(tasks)),
        distribution_(RandomGenerator::GetInstance(), std::move(weights)) {}

  template <typename Iter>
    requires std::forward_iterator<Iter>
  TaskGenerator(Tasks tasks, Iter weights_begin, Iter weights_end)
      : tasks_(std::move(tasks)),
        distribution_(RandomGenerator::GetInstance(), weights_begin,
                      weights_end) {}

  Task Get() {
    auto task_index = distribution_.Get();
    return tasks_[task_index];
  }

 private:
  Tasks tasks_;
  DiscreteDistribution distribution_;
};

}  // namespace aco