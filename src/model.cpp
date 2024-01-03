#include "model.hpp"

namespace aco {

Model::Model(const Warehouse& warehouse, const Weights& weights)
    : task_generator_(CreateTargets(warehouse.NumberFinishVertices()), weights),
      warehouse_(warehouse),
      robots_(num_robots_),
      robot_coord_(num_robots_),
      algorithm_(warehouse.GetStartVertices()[0], warehouse.GetFinishVertices(),
                 warehouse, robot_coord_) {
  for (auto& robot : robots_) {
    robot.SetAlgorithm(&algorithm_);
  }
}

void Model::Run() {
  while (completed_targets_ != num_targets_) {
    ++timer_;
    size_t robot_id = 0;
    for (auto& robot : robots_) {
      if (!robot.Busy()) {
        if (targets_started_ < num_targets_) {
          auto target = GetNextTarget();
          robot.SetRoute(target);
          ++targets_started_;
        }
      }

      auto robot_step = robot.MakeStep();
      robot_coord_[robot_id] = robot_step;

      if (robot.Finished()) {
        ++completed_targets_;
      }

      ++robot_id;
    }
  }
}

Target Model::GetNextTarget() { return task_generator_.Get(); }

uint64_t Model::GetTime() const { return timer_; }

}  // namespace aco