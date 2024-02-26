#include <robot.hpp>

namespace aco {

void Robot::SetAlgorithm(MultiTargetACO *algo) { algorithm_ = algo; }

void Robot::SetRoute(Target target) {
  start_ = algorithm_->GetStartByTarget(target);
  finish_ = algorithm_->GetFinishByTarget(target);
  route_ = {start_};
  target_ = target;
  to_finish_ = true;
  busy_ = true;
}

std::optional<Coordinates> Robot::MakeStep() {
  if (!Busy()) {
    return {};
  }
  auto step = algorithm_->MakeStepForTarget(target_, route_);
  route_.push_back(step);

  if (to_finish_ && route_.back() == finish_) {
    to_finish_ = false;
    algorithm_->UpdatePheromonesForTarget(target_, route_);
    route_ = {finish_};

  } else if (!to_finish_ && route_.back() == start_) {
    busy_ = false;
    algorithm_->UpdatePheromonesForTarget(target_, route_);
  }

  return step;
}

bool Robot::Busy() const { return busy_; }

bool Robot::Finished() const { return !Busy() && !route_.empty(); }

const Route &Robot::GetRoute() const { return route_; }

} // namespace aco