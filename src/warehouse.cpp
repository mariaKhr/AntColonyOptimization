#include "warehouse.hpp"

#include <cmath>
#include <unordered_set>
#include <variant>

#include "utils.hpp"

namespace aco {

WarehouseCeil CreateCeil(std::string_view type) {
  if (type.compare("R") == 0) {
    return UnavailableCeil();
  } else if (type.compare("G") == 0) {
    return EmptyCeil();
  } else if (type.compare("T") == 0) {
    return StartCeil();
  } else if (type.compare("Y") == 0) {
    return FinishCeil();
  } else {
    throw std::runtime_error(
        "Invalid ceil type. One of the following is expected: R, G, T, Y");
  }
}

std::ostream& operator<<(std::ostream& os, const WarehouseCeil& ceil) {
  std::visit(overloaded{[&os](const UnavailableCeil&) { os << "R"; },
                        [&os](const EmptyCeil&) { os << "G"; },
                        [&os](const StartCeil&) { os << "T"; },
                        [&os](const FinishCeil&) { os << "Y"; }},
             ceil);
  return os;
}

bool IsAvailable(const WarehouseCeil& ceil) {
  return !std::holds_alternative<UnavailableCeil>(ceil);
}

Warehouse::Warehouse(const std::vector<std::vector<std::string>>& map) {
  height_ = map.size();

  if (height_ != 0) {
    width_ = map[0].size();
  }

  if (height_ == 0 || width_ == 0) {
    throw std::runtime_error("Invalid map size");
  }

  for (const auto& row : map) {
    if (row.size() != width_) {
      throw std::runtime_error("Invalid row size");
    }
  }

  auto find_start_and_finish_ceil_visitor = overloaded{
      [](const UnavailableCeil&) {}, [](const EmptyCeil&) {},
      [this](const StartCeil&) -> void { start_ceil_ = ceils_.size() - 1; },
      [this](const FinishCeil&) -> void {
        finish_ceils_.push_back(ceils_.size() - 1);
      }};

  ceils_.reserve(height_ * width_);
  for (const auto& row : map) {
    for (const auto& ceil_type : row) {
      ceils_.emplace_back(CreateCeil(ceil_type));
      std::visit(find_start_and_finish_ceil_visitor, ceils_.back());
    }
  }
}

size_t Warehouse::GetStartVertex() const { return start_ceil_; }

const std::vector<size_t>& Warehouse::GetFinishVertexes() const {
  return finish_ceils_;
}

Graph Warehouse::ToGraph(const Warehouse& warehouse) {
  size_t num_vertex = warehouse.height_ * warehouse.width_;
  auto graph = Graph(num_vertex);
  for (size_t vertex = 0; vertex < num_vertex; ++vertex) {
    for (auto neigh : warehouse.GetNeighbours(vertex)) {
      graph.SetEdge(vertex, neigh, warehouse.GetDistance(vertex, neigh));
    }
  }
  return graph;
}

void Warehouse::VisualizeRoute(std::ostream& os, const Route& route) {
  std::unordered_set<Vertex> route_vertex(route.begin(), route.end());

  for (size_t i = 0; i < ceils_.size(); ++i) {
    if (route_vertex.contains(i)) {
      if (i == start_ceil_) {
        os << "T ";
      } else if (i == finish_ceils_[0]) {
        os << "Y ";
      } else {
        os << ". ";
      }

    } else {
      const auto& ceil = ceils_[i];
      os << ceil << ' ';
    }

    if (i % width_ + 1 == width_) {
      os << '\n';
    }
  }
}

std::vector<size_t> Warehouse::GetNeighbours(size_t vertex) const {
  if (!IsAvailable(ceils_[vertex])) {
    return {};
  }
  std::vector<size_t> neighbours;

  size_t row = vertex / width_;
  size_t col = vertex % width_;

  if (col != width_ - 1 && IsAvailable(ceils_[vertex + 1])) {
    neighbours.push_back(vertex + 1);
  }
  if (col != 0 && IsAvailable(ceils_[vertex - 1])) {
    neighbours.push_back(vertex - 1);
  }
  if (row != height_ - 1 && IsAvailable(ceils_[vertex + width_])) {
    neighbours.push_back(vertex + width_);
  }
  if (row != 0 && IsAvailable(ceils_[vertex - width_])) {
    neighbours.push_back(vertex - width_);
  }

  return neighbours;
}

Distance Warehouse::GetDistance(size_t from, size_t to) const {
  size_t from_x = from % width_;
  size_t from_y = from / width_;
  size_t to_x = to % width_;
  size_t to_y = to / width_;

  double delta_x = static_cast<Distance>(from_x) - static_cast<Distance>(to_x);
  double delta_y = static_cast<Distance>(from_y) - static_cast<Distance>(to_y);

  return std::sqrt(delta_x * delta_x + delta_y * delta_y);
}

std::ostream& operator<<(std::ostream& os, const Warehouse& warehouse) {
  for (size_t i = 0; i < warehouse.ceils_.size(); ++i) {
    const auto& ceil = warehouse.ceils_[i];
    os << ceil << ' ';
    if (i % warehouse.width_ + 1 == warehouse.width_) {
      os << '\n';
    }
  }
  return os;
}

}  // namespace aco