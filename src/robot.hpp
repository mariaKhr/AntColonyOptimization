#pragma once

#include "ACO.hpp"

namespace aco {

class Robot final {
 public:
  void SetAlgorithm(MultiTargetACO* algo);
  void SetRoute(Target target);
  std::optional<Coordinates> MakeStep();

  bool Busy() const;
  bool Finished() const;

 private:
  Route route_;
  Coordinates start_;
  Coordinates finish_;
  Target target_;

  bool to_finish_;
  bool busy_ = false;
  MultiTargetACO* algorithm_;
};

}  // namespace aco