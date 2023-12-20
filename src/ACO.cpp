#include "ACO.hpp"
#include "task_generator.hpp"

#include <optional>
#include <unordered_set>

namespace aco {

Pheromones Pheromones::PheromonesWithoutInitialValue(size_t num_vertex, double ro) {
  return Pheromones(num_vertex, ro);
}

Pheromones Pheromones::PheromonesWithInitialValue(size_t num_vertex, double ro, PheromoneType initial_value) {
  return Pheromones(num_vertex, ro, /*initial_filling*/ true, initial_value);
}

Pheromones::Pheromones(size_t num_vertex, double ro, bool initial_filling, PheromoneType initial_value)
    : pheromones_(num_vertex), ro_(ro) {
  if (initial_filling) {
    for (size_t from = 0; from < num_vertex; ++from) {
      for (size_t to = 0; to < num_vertex; ++to) {
        pheromones_.Set(from, to, initial_value);
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

void Pheromones::Add(Vertex from, Vertex to, PheromoneType delta) {
  pheromones_.Add(from, to, delta);
}

IACO::IACO(ACOParameters parameters, Graph graph)
    : parameters_(std::move(parameters)),
      graph_(std::move(graph)),
      pheromones_(Pheromones::PheromonesWithInitialValue(graph_.Size(), parameters_.ro, parameters.initial_pheromone)) {}

BasicACO::BasicACO(ACOParameters parameters, Graph graph)
    : IACO(std::move(parameters), std::move(graph)),
      routes_(parameters_.ants),
      best_length_(std::nullopt) {}

Route BasicACO::Execute() {
  for (uint32_t i = 0; i < parameters_.num_iter; ++i) {
    MakeIteration();
  }
  return best_route_;
}

void BasicACO::MakeIteration() {
  for (uint32_t ant = 0; ant < parameters_.ants; ++ant) {
    while (!ValidRoute(routes_[ant])) {
      routes_[ant] = FindRoute(ant);
    }

    auto length = RouteLength(routes_[ant]);
    if (!best_length_ || length < *best_length_) {
      best_length_ = length;
      best_route_ = routes_[ant];
    }
  }

  UpdatePheromones();

  for (uint32_t ant; ant < parameters_.ants; ++ant) {
    routes_[ant].clear();
  }
}

void BasicACO::UpdatePheromones() {
  auto pheromone_deltas = Pheromones::PheromonesWithoutInitialValue(graph_.Size(), 0);

  for (const auto &route : routes_) {
    auto length = RouteLength(route);
    auto delta = parameters_.q / length;
    for (size_t i = 0; i + 1 < route.size(); ++i) {
      auto from = route[i];
      auto to = route[i + 1];
      pheromone_deltas.Add(from, to, delta);
      pheromone_deltas.Add(to, from, delta);
    }
  }

  pheromones_.Update(pheromone_deltas);
}

Route BasicACO::FindRoute(uint32_t ant) const {
  Route route{parameters_.start_vertex};
  std::unordered_set<Vertex> visited{route.back()};

  while (route.back() != parameters_.finish_vertex) {
    std::vector<double> probs;
    std::vector<Vertex> valid_neigh;

    {
      auto neighbours = graph_.GetNeighbours(route.back());
      for (const auto &neig : neighbours) {
        if (visited.contains(neig)) {
          continue;
        }

        auto eta =
            std::pow(1 / graph_.GetEdge(route.back(), neig), parameters_.beta);
        auto tau = std::pow(pheromones_.GetPheromone(route.back(), neig),
                            parameters_.alpha);

        probs.push_back(eta * tau);
        valid_neigh.push_back(neig);
      }
    }

    double sum = 0;
    for (const auto &prob : probs) {
      sum += prob;
    }

    for (auto &prob : probs) {
      prob /= sum;
    }

    // deadlock
    if (probs.empty()) {
      break;
    }

    auto next_vertex =
        TaskGenerator(valid_neigh, RandomGenerator::GetInstance(),
                      probs.begin(), probs.end())
            .Get();
    route.push_back(next_vertex);
    visited.insert(next_vertex);
  }

  return route;
}

Distance BasicACO::RouteLength(const Route &route) const {
  Distance sum = 0;
  for (size_t i = 0; i + 1 < route.size(); ++i) {
    auto from = route[i];
    auto to = route[i + 1];
    sum += graph_.GetEdge(from, to);
  }
  return sum;
}

bool BasicACO::ValidRoute(const Route &route) const {
  if (route.empty() || route[0] != parameters_.start_vertex ||
      route.back() != parameters_.finish_vertex) {
    return false;
  }
  return true;
}

}  // namespace aco