#include "ACO.hpp"

#include "task_generator.hpp"

namespace aco {

BasicACO::BasicACO(ACOParameters parameters, const Graph &graph)
    : parameters_(std::move(parameters)),
      graph_(std::move(graph)),
      pheromones_(Pheromones::PheromonesWithInitialValue(
          graph_.Size(), parameters_.ro, parameters.initial_pheromone)) {}

Route BasicACO::Execute() {
  auto route = FindRoute();
  UpdatePheromones(route);
  return route;
}

Route BasicACO::FindRoute() const {
  Route route;
  while (!IsValidRoute(route)) {
    route = TryFindRoute();
  }
  return route;
}

Route BasicACO::TryFindRoute() const {
  Route route{parameters_.start_vertex};
  std::unordered_set<Vertex> visited{route.back()};

  while (route.back() != parameters_.finish_vertex) {
    auto cur_vertex = route.back();
    auto available_neigh = GetAvailableNeighbors(cur_vertex, visited);

    // deadlock
    if (available_neigh.empty()) {
      break;
    }

    auto probs = CalculateTransitionProbabilities(cur_vertex, available_neigh);
    auto next_vertex =
        TaskGenerator(available_neigh, probs.begin(), probs.end()).Get();
    route.push_back(next_vertex);
    visited.insert(next_vertex);
  }

  return route;
}

std::vector<Vertex> BasicACO::GetAvailableNeighbors(
    Vertex vertex, const std::unordered_set<Vertex> &forbidden) const {
  std::vector<Vertex> available_neigh;

  auto neighbours = graph_.GetNeighbours(vertex);
  for (const auto &neig : neighbours) {
    if (!forbidden.contains(neig)) {
      available_neigh.push_back(neig);
    }
  }

  return available_neigh;
}

std::vector<double> BasicACO::CalculateTransitionProbabilities(
    Vertex start_vertex, const std::vector<Vertex> &neighbors) const {
  std::vector<double> probs;
  probs.reserve(neighbors.size());

  for (const auto &neig : neighbors) {
    auto edge_len = graph_.GetEdge(start_vertex, neig);
    auto pheromone_count = pheromones_.GetPheromone(start_vertex, neig);
    auto eta = CalculateEta(edge_len);
    auto tau = CalculateTau(pheromone_count);
    probs.push_back(eta * tau);
  }

  return probs;
}

double BasicACO::CalculateEta(Distance edge_len) const {
  return std::pow(1 / edge_len, parameters_.beta);
}

double BasicACO::CalculateTau(PheromoneType pheromone_count) const {
  return std::pow(pheromone_count, parameters_.alpha);
}

Distance BasicACO::CalculateRouteLength(const Route &route) const {
  Distance sum = 0;

  for (size_t i = 0; i + 1 < route.size(); ++i) {
    auto from = route[i];
    auto to = route[i + 1];
    sum += graph_.GetEdge(from, to);
  }

  return sum;
}

bool BasicACO::IsValidRoute(const Route &route) const {
  if (route.empty() || route[0] != parameters_.start_vertex ||
      route.back() != parameters_.finish_vertex) {
    return false;
  }
  return true;
}

void BasicACO::UpdatePheromones(const Route &route) {
  auto pheromone_deltas =
      Pheromones::PheromonesWithoutInitialValue(graph_.Size(), 0);

  auto length = CalculateRouteLength(route);
  auto delta = parameters_.q / length;
  for (size_t i = 0; i + 1 < route.size(); ++i) {
    auto from = route[i];
    auto to = route[i + 1];
    pheromone_deltas.AddPheromone(from, to, delta);
    pheromone_deltas.AddPheromone(to, from, delta);
  }

  pheromones_.Update(pheromone_deltas);
}

}  // namespace aco