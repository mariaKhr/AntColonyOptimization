#pragma once

#include <cstdint>
#include <functional>

namespace aco {

struct Coordinates {
  uint32_t x;
  uint32_t y;

  auto operator<=>(const Coordinates &other) const = default;
};

struct CoordinatesHash {
  std::size_t operator()(const Coordinates &c) const {
    return std::hash<uint32_t>()(c.x) ^ std::hash<uint32_t>()(c.y);
  }
};

} // namespace aco