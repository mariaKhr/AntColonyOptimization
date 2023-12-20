#pragma once

#include <unordered_set>

#include "graph.hpp"
#include "pheromones.hpp"

namespace aco {

struct ACOParameters {
  double alpha = 0.9;  // parameter to control the influence of pheromones
  double beta = 0.1;   // parameter to control the influence of distance
  double q = 1;        // estimation of the suspected best route
  double ro = 0.1;     // pheromone evaporation coefficient
  double initial_pheromone = 0.1;
  uint32_t start_vertex;
  uint32_t finish_vertex;
};

class BasicACO final {
 public:
  BasicACO(ACOParameters parameters, const Graph &graph);
  ~BasicACO() = default;

  Route Execute();

 private:
  Route FindRoute() const;
  Route TryFindRoute() const;
  Distance CalculateRouteLength(const Route &route) const;
  bool IsValidRoute(const Route &route) const;

  std::vector<Vertex> GetAvailableNeighbors(
      Vertex vertex, const std::unordered_set<Vertex> &forbidden) const;
  std::vector<double> CalculateTransitionProbabilities(
      Vertex start_vertex, const std::vector<Vertex> &neighbors) const;

  double CalculateEta(Distance edge_len) const;
  double CalculateTau(PheromoneType pheromone_count) const;

  void UpdatePheromones(const Route &route);

 private:
  ACOParameters parameters_;
  const Graph &graph_;
  Pheromones pheromones_;
};

using Target = uint32_t;

class MultiTargetACO final {
 public:
  MultiTargetACO(std::vector<ACOParameters> parameters, const Graph &graph);

  Route Execute(Target target);

 private:
  std::vector<BasicACO> algorithms_;
};

}  // namespace aco