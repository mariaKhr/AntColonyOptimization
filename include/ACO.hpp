#pragma once

#include <optional>
#include <pheromones.hpp>
#include <task_generator.hpp>
#include <warehouse.hpp>

namespace aco {

struct BasicACOParameters {
  double alpha = 2.0; // parameter to control the influence of pheromones
  double ro = 0.1;    // pheromone evaporation coefficient
  double initial_pheromone = 0.01;
  Coordinates start;
  Coordinates finish;
};

// Algorithm works for one start and one finish vertex
class BasicACO final {
public:
  BasicACO(BasicACOParameters parameters, const Warehouse &warehouse,
           const std::vector<std::optional<Coordinates>> &robots);

  Coordinates MakeStep(const Route &route) const;
  void UpdatePheromones(const Route &route);
  Coordinates GetStart() const;
  Coordinates GetFinish() const;

private:
  std::vector<Coordinates> GetAvailableNeighbors(Coordinates coord,
                                                 const Route &route) const;
  std::vector<double> CalculateTransitionProbabilities(
      Coordinates start_vertex,
      const std::vector<Coordinates> &neighbors) const;
  double CalculateTau(PheromoneType pheromone_count) const;
  bool AreRobotsInCoordinates(Coordinates coord) const;

private:
  BasicACOParameters parameters_;
  const Warehouse &warehouse_;
  Pheromones pheromones_;
  const std::vector<std::optional<Coordinates>> &robots_;
};

// Algorithm works for one start and several finish vertices
class MultiTargetACO final {
public:
  MultiTargetACO(Coordinates start, const std::vector<Coordinates> &targets,
                 const Warehouse &warehouse,
                 const std::vector<std::optional<Coordinates>> &robots);

  Coordinates MakeStepForTarget(Target target, const Route &route) const;
  void UpdatePheromonesForTarget(Target target, const Route &route);
  Coordinates GetStartByTarget(Target target) const;
  Coordinates GetFinishByTarget(Target target) const;

private:
  std::vector<BasicACO> algorithms_;
};

} // namespace aco