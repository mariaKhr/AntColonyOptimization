#include <pheromones.hpp>

namespace aco {

Pheromones::Pheromones(uint32_t height, uint32_t width,
                       PheromoneType initial_value) {
  pheromones_.assign(height, std::vector<PheromoneType>(width, initial_value));
}

PheromoneType Pheromones::Get(Coordinates coord) const {
  return pheromones_[coord.x][coord.y];
}

void Pheromones::Set(Coordinates coord, PheromoneType value) {
  pheromones_[coord.x][coord.y] = value;
}

void Pheromones::Add(Coordinates coord, PheromoneType delta) {
  pheromones_[coord.x][coord.y] += delta;
}

void Pheromones::Evaporate(double evaporation_coef) {
  for (auto &row : pheromones_) {
    for (auto &ceil : row) {
      ceil *= (1 - evaporation_coef);
    }
  }
}

} // namespace aco