#pragma once

#include <memory>
#include <ostream>
#include <optional>
#include <variant>

#include "csv.hpp"
#include "graph.hpp"

namespace aco {

struct EmptyCeil {};

struct StartCeil {};

struct UnavailableCeil {};

struct FinishCeil {
  uint32_t number;
};

using WarehouseCeil =
    std::variant<EmptyCeil, StartCeil, UnavailableCeil, FinishCeil>;

WarehouseCeil CreateCeil(std::string_view type);
std::ostream& operator<<(std::ostream& os, const WarehouseCeil& obj);

bool IsAvailable(const WarehouseCeil& ceil);

class Warehouse final {
 public:
  explicit Warehouse(const Rows& warehouse);

  size_t GetStartVertex() const;
  const std::vector<size_t>& GetFinishVertexes() const;

  static Graph ToGraph(const Warehouse& warehouse);
  void VisualizeRoute(std::ostream& os, const Route& route);

 private:
  std::vector<size_t> GetNeighbours(size_t vertex) const;
  Distance GetDistance(size_t from, size_t to) const;

 private:
  size_t height_;
  size_t width_;

  std::vector<WarehouseCeil> ceils_;
  std::optional<size_t> start_ceil_;
  std::vector<size_t> finish_ceils_;

  friend std::ostream& operator<<(std::ostream& os, const Warehouse& warehouse);
};

std::ostream& operator<<(std::ostream& os, const Warehouse& warehouse);

}  // namespace aco