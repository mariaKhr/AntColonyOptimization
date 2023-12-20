#pragma once

#include <unordered_set>

#include "graph.hpp"
#include "pheromones.hpp"

namespace aco {

struct ACOParameters {
  uint32_t ants;
  uint32_t num_iter;
  double alpha;  // parameter to control the influence of pheromones
  double beta;   // parameter to control the influence of distance
  double q;      // estimation of the suspected best route
  double ro;     // pheromone evaporation coefficient
  double initial_pheromone;
  uint32_t start_vertex;
  uint32_t finish_vertex;
};

class BasicACO final {
 public:
  BasicACO(ACOParameters parameters, Graph graph);
  ~BasicACO() = default;

  void Execute();
  const Route &GetBestRoute() const;

 private:
  void MakeIteration();

  Route FindRoute() const;
  Route TryFindRoute() const;
  void UpdateBestRoute(const Route &route);
  Distance CalculateRouteLength(const Route &route) const;
  bool IsValidRoute(const Route &route) const;

  std::vector<Vertex> GetAvailableNeighbors(
      Vertex vertex, const std::unordered_set<Vertex> &forbidden) const;
  std::vector<double> CalculateTransitionProbabilities(
      Vertex start_vertex, const std::vector<Vertex> &neighbors) const;

  double CalculateEta(Distance edge_len) const;
  double CalculateTau(PheromoneType pheromone_count) const;

  void UpdatePheromones();

 private:
  ACOParameters parameters_;
  Graph graph_;
  Pheromones pheromones_;

  std::vector<Route> ant_routes_;
  Route best_route_;
  std::optional<Distance> best_length_;
};

}  // namespace aco