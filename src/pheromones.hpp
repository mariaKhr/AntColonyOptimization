#pragma once

#include <optional>

#include "graph.hpp"

namespace aco {

using PheromoneType = double;

class Pheromones final {
 public:
  static Pheromones PheromonesWithoutInitialValue(size_t num_vertex, double ro);
  static Pheromones PheromonesWithInitialValue(size_t num_vertex, double ro,
                                               PheromoneType initial_value);

  PheromoneType GetPheromone(Vertex from, Vertex to) const;
  void AddPheromone(Vertex from, Vertex to, PheromoneType delta);
  void Update(const Pheromones &delta);

 private:
  Pheromones(size_t num_vertex, double ro,
             std::optional<PheromoneType> initial_value = std::nullopt);

 private:
  AdjacencyMatrix<PheromoneType> pheromones_;
  double ro_;
};

}  // namespace aco