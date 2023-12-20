#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <unordered_set>
#include <variant>

#include "csv.hpp"
#include "graph.hpp"

namespace aco {

enum class WarehouseCeilType {
  empty,
  unavailable,
  start,
  finish,
};

bool IsAvailableWarehouseCeil(WarehouseCeilType ceil);

WarehouseCeilType CreateWarehouseCeil(std::string_view type);
std::ostream &operator<<(std::ostream &os, WarehouseCeilType obj);

class Warehouse final {
 public:
  explicit Warehouse(const Rows &warehouse);

  Vertex GetStartVertex() const;
  const std::unordered_set<Vertex> &GetFinishVertices() const;

  static Graph ToGraph(const Warehouse &warehouse);
  void VisualizeRoute(std::ostream &os, const Route &route);

 private:
  std::vector<Vertex> GetNeighbours(Vertex vertex) const;
  Distance GetDistance(Vertex from, Vertex to) const;

 private:
  size_t height_;
  size_t width_;

  std::vector<WarehouseCeilType> ceils_;
  std::optional<Vertex> start_ceil_;
  std::unordered_set<Vertex> finish_ceils_;

  friend std::ostream &operator<<(std::ostream &os, const Warehouse &warehouse);
};

std::ostream &operator<<(std::ostream &os, const Warehouse &warehouse);

}  // namespace aco