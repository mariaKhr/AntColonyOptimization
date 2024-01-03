#pragma once

#include "robot.hpp"

namespace aco {

class Model final {
 public:
  Model(const Warehouse& warehouse, const Weights& weights);

  void Run();
  uint64_t GetTime() const;

 private:
  Target GetNextTarget();

 private:
  TaskGenerator<Target> task_generator_;
  const Warehouse& warehouse_;
  const uint32_t num_robots_ = 8;
  const uint32_t num_targets_ = 10000;

  uint32_t targets_started_;
  uint32_t completed_targets_;
  std::vector<Robot> robots_;
  std::vector<std::optional<Coordinates>> robot_coord_;
  uint64_t timer_;

  MultiTargetACO algorithm_;
};

}  // namespace aco