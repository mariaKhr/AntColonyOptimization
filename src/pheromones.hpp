#pragma once

#include "coordinates.hpp"

namespace aco {

using PheromoneType = double;

class Pheromones final {
 public:
  Pheromones(uint32_t height, uint32_t width, PheromoneType initial_value);

  PheromoneType Get(Coordinates coord) const;
  void Set(Coordinates coord, PheromoneType value);
  void Add(Coordinates coord, PheromoneType delta);
  void Evaporate(double evaporation_coef);

 private:
  std::vector<std::vector<PheromoneType>> pheromones_;
};

}  // namespace aco