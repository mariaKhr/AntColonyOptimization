#include <ACO.hpp>

namespace aco {

BasicACO::BasicACO(BasicACOParameters parameters, const Warehouse &warehouse,
                   const std::vector<std::optional<Coordinates>> &robots)
    : parameters_(std::move(parameters)), warehouse_(warehouse),
      pheromones_(warehouse_.Height(), warehouse_.Width(),
                  parameters_.initial_pheromone),
      robots_(robots) {}

Coordinates BasicACO::MakeStep(const Route &route) const {
  auto available_neigh = GetAvailableNeighbors(route.back(), route);

  if (available_neigh.empty()) {
    available_neigh = GetAvailableNeighbors(route.back(), {route.back()});
    // deadlock
    if (available_neigh.empty()) {
      return route.back();
    }
    auto probs =
        CalculateTransitionProbabilities(route.back(), available_neigh);
    auto next_vertex = TaskGenerator(available_neigh, std::move(probs)).Get();
    return next_vertex;
  }

  auto probs = CalculateTransitionProbabilities(route.back(), available_neigh);
  auto next_vertex = TaskGenerator(available_neigh, std::move(probs)).Get();
  return next_vertex;
}

void BasicACO::UpdatePheromones(const Route &route) {
  pheromones_.Evaporate(parameters_.ro);

  auto length = route.size();
  auto delta = 1.0 / length;
  for (const auto &coord : route) {
    pheromones_.Add(coord, delta);
  }
}

Coordinates BasicACO::GetStart() const { return parameters_.start; }

Coordinates BasicACO::GetFinish() const { return parameters_.finish; }

std::vector<Coordinates>
BasicACO::GetAvailableNeighbors(Coordinates coord, const Route &route) const {
  std::vector<Coordinates> available_neigh;

  auto neighbours = warehouse_.GetNeighbours(coord);
  for (const auto &neig : neighbours) {
    if (std::find(route.begin(), route.end(), neig) == route.end() &&
        !AreRobotsInCoordinates(neig)) {
      available_neigh.push_back(neig);
    }
  }

  return available_neigh;
}

std::vector<double> BasicACO::CalculateTransitionProbabilities(
    Coordinates start_vertex, const std::vector<Coordinates> &neighbors) const {
  std::vector<double> probs;
  probs.reserve(neighbors.size());

  for (const auto &neig : neighbors) {
    auto pheromone_count = pheromones_.Get(neig);
    auto tau = CalculateTau(pheromone_count);
    probs.push_back(tau);
  }

  return probs;
}

double BasicACO::CalculateTau(PheromoneType pheromone_count) const {
  return std::pow(pheromone_count, parameters_.alpha);
}

bool BasicACO::AreRobotsInCoordinates(Coordinates coord) const {
  return std::find(robots_.begin(), robots_.end(), coord) != robots_.end();
}

MultiTargetACO::MultiTargetACO(
    Coordinates start, const std::vector<Coordinates> &targets,
    const Warehouse &warehouse,
    const std::vector<std::optional<Coordinates>> &robots) {
  algorithms_.reserve(targets.size());
  for (const auto &target : targets) {
    algorithms_.emplace_back(
        BasicACOParameters{.start = start, .finish = target}, warehouse,
        robots);
  }
}

Coordinates MultiTargetACO::MakeStepForTarget(Target target,
                                              const Route &route) const {
  return algorithms_[target].MakeStep(route);
}

void MultiTargetACO::UpdatePheromonesForTarget(Target target,
                                               const Route &route) {
  algorithms_[target].UpdatePheromones(route);
}

Coordinates MultiTargetACO::GetStartByTarget(Target target) const {
  return algorithms_[target].GetStart();
}

Coordinates MultiTargetACO::GetFinishByTarget(Target target) const {
  return algorithms_[target].GetFinish();
}

} // namespace aco