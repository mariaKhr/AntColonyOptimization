#pragma once

#include <optional>

#include "graph.hpp"

namespace aco {

using PheromoneType = double;

class Pheromones final {
 public:
  Pheromones(size_t num, double ro, bool initial_filling = false, double taumax = 0.0);

  PheromoneType GetPheromone(Vertex from, Vertex to) const;
  void Add(Vertex from, Vertex to, PheromoneType delta);

  void Update(const Pheromones& delta);

 private:
  AdjacencyMatrix<PheromoneType> pheromones_;
  double ro_;
};

struct ACOParameters {
  uint32_t ants;
  uint32_t num_iter;
  double alpha; // parameter to control the influence of pheromones
  double beta; // parameter to control the influence of distance
  double q; // estimation of the suspected best route
  double ro; // pheromone evaporation coefficient
  double taumax; // maximum pheromone random number
  uint32_t start_vertex;
  uint32_t finish_vertex;
};

class IACO {
 public:
  IACO(ACOParameters parameters, Graph graph);
  virtual ~IACO() = default;

  virtual Route Execute() = 0;

 protected:
  ACOParameters parameters_;
  Graph graph_;
  Pheromones pheromones_;
};

class BasicACO : public IACO {
 public:
  BasicACO(ACOParameters parameters, Graph graph);
  ~BasicACO() = default;

  Route Execute() override;

 private:
  void MakeIteration();
  void UpdatePheromones();

  Route FindRoute(uint32_t ant) const;
  Distance RouteLength(const Route& route) const;
  bool ValidRoute(const Route& route) const;

 private:
  std::vector<Route> routes_;

  Route best_route_;
  std::optional<Distance> best_length_;
};

}  // namespace aco