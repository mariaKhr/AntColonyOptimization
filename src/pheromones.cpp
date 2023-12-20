#include "pheromones.hpp"

namespace aco {

Pheromones Pheromones::PheromonesWithoutInitialValue(size_t num_vertex,
                                                     double ro) {
  return Pheromones(num_vertex, ro);
}

Pheromones Pheromones::PheromonesWithInitialValue(size_t num_vertex, double ro,
                                                  PheromoneType initial_value) {
  return Pheromones(num_vertex, ro, initial_value);
}

Pheromones::Pheromones(size_t num_vertex, double ro,
                       std::optional<PheromoneType> initial_value)
    : pheromones_(num_vertex), ro_(ro) {
  if (initial_value) {
    for (size_t from = 0; from < num_vertex; ++from) {
      for (size_t to = 0; to < num_vertex; ++to) {
        pheromones_.Set(from, to, *initial_value);
      }
    }
  }
}

PheromoneType Pheromones::GetPheromone(Vertex from, Vertex to) const {
  return pheromones_.Get(from, to);
}

void Pheromones::Update(const Pheromones &delta) {
  for (size_t from = 0; from < pheromones_.Size(); ++from) {
    for (size_t to = 0; to < pheromones_.Size(); ++to) {
      auto cur_pheromone = pheromones_.Get(from, to);
      auto new_pheromone =
          cur_pheromone * (1 - ro_) + delta.GetPheromone(from, to);
      pheromones_.Set(from, to, new_pheromone);
    }
  }
}

void Pheromones::AddPheromone(Vertex from, Vertex to, PheromoneType delta) {
  pheromones_.Add(from, to, delta);
}

}  // namespace aco