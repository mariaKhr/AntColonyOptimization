#pragma once

#include <memory>

#include "coordinates.hpp"

namespace aco {

enum class WarehouseCeilType {
  empty,
  unavailable,
  start,
  finish,
};

class WarehouseCeil final {
 public:
  WarehouseCeil(std::string_view type);

  WarehouseCeilType GetType() const;
  bool IsAvailable() const;

 private:
  WarehouseCeilType type_;
};

class Warehouse final {
 public:
  Warehouse(const std::vector<std::vector<std::string>> &warehouse);
  static Warehouse WarehouseFromFile(std::string_view filepath);

  uint32_t Height() const;
  uint32_t Width() const;

  const std::vector<Coordinates> &GetStartVertices() const &;
  const std::vector<Coordinates> &GetFinishVertices() const &;
  size_t NumberStartVertices() const;
  size_t NumberFinishVertices() const;
  std::vector<Coordinates> GetNeighbours(Coordinates coord) const;

 private:
  std::vector<std::vector<WarehouseCeil>> warehouse_;
  std::vector<Coordinates> start_ceils_;
  std::vector<Coordinates> finish_ceils_;
};

using Route = std::vector<Coordinates>;
using RoutePtr = std::unique_ptr<Route>;

}  // namespace aco