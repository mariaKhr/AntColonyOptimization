#pragma once

#include <optional>
#include <unordered_set>

#include "pheromones.hpp"
#include "task_generator.hpp"
#include "warehouse.hpp"

namespace aco {

class MultiTargetACO final {
 public:
  MultiTargetACO(Coordinates start, const std::vector<Coordinates> &targets,
                 const Warehouse &warehouse,
                 const std::vector<std::optional<Coordinates>> &robots);

  Coordinates MakeStep(Target target, const Route &route) const;
  void UpdatePheromones(Target target, const Route &route);

  Coordinates GetStart(Target target) const;
  Coordinates GetFinish(Target target) const;

 private:
  struct BasicACOParameters {
    double alpha = 1.0;  // parameter to control the influence of pheromones
    double ro = 0.1;    // pheromone evaporation coefficient
    double initial_pheromone = 0.01;
    Coordinates start;
    Coordinates finish;
  };

  class BasicACO {
   public:
    BasicACO(BasicACOParameters parameters, const Warehouse &warehouse,
             const std::vector<std::optional<Coordinates>> &robots);
    ~BasicACO() = default;

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

    bool NoRobots(Coordinates coord) const;

   private:
    BasicACOParameters parameters_;
    const Warehouse &warehouse_;
    Pheromones pheromones_;
    const std::vector<std::optional<Coordinates>> &robots_;
  };

 private:
  std::vector<std::unique_ptr<BasicACO>> algorithms_;
};

}  // namespace aco