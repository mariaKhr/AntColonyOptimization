#include "warehouse.hpp"

#include <cassert>
#include <cmath>
#include <unordered_set>
#include <variant>

namespace aco {

bool IsAvailableWarehouseCeil(WarehouseCeilType ceil) {
  return ceil != WarehouseCeilType::unavailable;
}

WarehouseCeilType CreateWarehouseCeil(std::string_view type) {
  if (type.compare("R") == 0) {
    return WarehouseCeilType::unavailable;
  } else if (type.compare("G") == 0) {
    return WarehouseCeilType::empty;
  } else if (type.compare("T") == 0) {
    return WarehouseCeilType::start;
  } else if (type.compare("Y") == 0) {
    return WarehouseCeilType::finish;
  } else {
    throw std::runtime_error(
        "Invalid ceil type. One of the following is expected: R, G, T, Y");
  }
}

std::ostream &operator<<(std::ostream &os, WarehouseCeilType ceil) {
  switch (ceil) {
    case WarehouseCeilType::unavailable:
      os << "R";
      break;
    case WarehouseCeilType::empty:
      os << "G";
      break;
    case WarehouseCeilType::start:
      os << "T";
      break;
    case WarehouseCeilType::finish:
      os << "Y";
      break;
    default:
      assert(0);
  }
  return os;
}

Warehouse::Warehouse(const Rows &map) {
  height_ = map.size();

  if (height_ != 0) {
    width_ = map[0].size();
  }

  if (height_ == 0 || width_ == 0) {
    throw std::runtime_error("Invalid map size");
  }

  for (const auto &row : map) {
    if (row.size() != width_) {
      throw std::runtime_error("Invalid row size");
    }
  }

  ceils_.reserve(height_ * width_);
  for (const auto &row : map) {
    for (const auto &ceil_type : row) {
      ceils_.emplace_back(CreateWarehouseCeil(ceil_type));

      const auto &new_ceil = ceils_.back();
      Vertex new_ceil_number = ceils_.size() - 1;

      switch (new_ceil) {
        case WarehouseCeilType::start:
          start_ceil_ = new_ceil_number;
          break;
        case WarehouseCeilType::finish:
          finish_ceils_.insert(new_ceil_number);
          break;
        default:
          break;
      }
    }
  }

  if (!start_ceil_ || finish_ceils_.empty()) {
    throw std::runtime_error("Invalid map. At least one T and Y was expected");
  }
}

size_t Warehouse::GetStartVertex() const { return *start_ceil_; }

const std::unordered_set<size_t> &Warehouse::GetFinishVertexes() const {
  return finish_ceils_;
}

Graph Warehouse::ToGraph(const Warehouse &warehouse) {
  size_t num_vertex = warehouse.height_ * warehouse.width_;
  auto graph = Graph(num_vertex);
  for (size_t vertex = 0; vertex < num_vertex; ++vertex) {
    for (const auto& neigh : warehouse.GetNeighbours(vertex)) {
      graph.SetEdge(vertex, neigh, warehouse.GetDistance(vertex, neigh));
    }
  }
  return graph;
}

void Warehouse::VisualizeRoute(std::ostream &os, const Route &route) {
  std::unordered_set<Vertex> route_vertices(route.begin(), route.end());

  for (size_t ceil_index = 0, num_ceils = ceils_.size(); ceil_index < num_ceils; ++ceil_index) {
    if (route_vertices.contains(ceil_index)) {
      if (ceil_index == start_ceil_) {
        os << "T ";
      } else if (finish_ceils_.contains(ceil_index)) {
        os << "Y ";
      } else {
        os << ". ";
      }

    } else {
      const auto &ceil = ceils_[ceil_index];
      os << ceil << ' ';
    }

    if (ceil_index % width_ + 1 == width_) {
      os << '\n';
    }
  }
}

std::vector<size_t> Warehouse::GetNeighbours(size_t vertex) const {
  if (!IsAvailableWarehouseCeil(ceils_[vertex])) {
    return {};
  }
  std::vector<size_t> neighbours;

  auto row = vertex / width_;
  auto col = vertex % width_;

  if (col != width_ - 1 && IsAvailableWarehouseCeil(ceils_[vertex + 1])) {
    neighbours.push_back(vertex + 1);
  }
  if (col != 0 && IsAvailableWarehouseCeil(ceils_[vertex - 1])) {
    neighbours.push_back(vertex - 1);
  }
  if (row != height_ - 1 && IsAvailableWarehouseCeil(ceils_[vertex + width_])) {
    neighbours.push_back(vertex + width_);
  }
  if (row != 0 && IsAvailableWarehouseCeil(ceils_[vertex - width_])) {
    neighbours.push_back(vertex - width_);
  }

  return neighbours;
}

Distance Warehouse::GetDistance(size_t from, size_t to) const {
  auto from_x = from % width_;
  auto from_y = from / width_;
  auto to_x = to % width_;
  auto to_y = to / width_;

  auto delta_x = static_cast<Distance>(from_x) - static_cast<Distance>(to_x);
  auto delta_y = static_cast<Distance>(from_y) - static_cast<Distance>(to_y);

  return std::sqrt(delta_x * delta_x + delta_y * delta_y);
}

std::ostream &operator<<(std::ostream &os, const Warehouse &warehouse) {
  for (size_t i = 0, size = warehouse.ceils_.size(); i < size; ++i) {
    const auto &ceil = warehouse.ceils_[i];
    os << ceil << ' ';
    if (i % warehouse.width_ + 1 == warehouse.width_) {
      os << '\n';
    }
  }
  return os;
}

}  // namespace aco