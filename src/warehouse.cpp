#include "warehouse.hpp"

#include <fstream>
#include <sstream>

namespace aco {

WarehouseCeil::WarehouseCeil(std::string_view type) {
  if (type.compare("R") == 0) {
    type_ = WarehouseCeilType::unavailable;
  } else if (type.compare("G") == 0) {
    type_ = WarehouseCeilType::empty;
  } else if (type.compare("T") == 0) {
    type_ = WarehouseCeilType::start;
  } else if (type.compare("Y") == 0) {
    type_ = WarehouseCeilType::finish;
  } else {
    throw std::runtime_error(
        "Invalid ceil type. One of the following is expected: R, G, T, Y");
  }
}

WarehouseCeilType WarehouseCeil::GetType() const { return type_; }

bool WarehouseCeil::IsAvailable() const {
  return type_ != WarehouseCeilType::unavailable;
}

Warehouse::Warehouse(const std::vector<std::vector<std::string>> &map) {
  size_t height = map.size();
  size_t width{};

  if (height != 0) {
    width = map[0].size();
  }
  if (height == 0 || width == 0) {
    throw std::runtime_error("Invalid map size");
  }

  for (const auto &row : map) {
    if (row.size() != width) {
      throw std::runtime_error("Invalid row size");
    }
  }

  warehouse_.assign(height, {});
  uint32_t row_ind = 0;
  uint32_t col_ind = 0;
  for (const auto &row : map) {
    col_ind = 0;
    for (const auto &ceil_type : row) {
      warehouse_[row_ind].emplace_back(ceil_type);
      const auto &new_ceil = warehouse_[row_ind].back();
      switch (new_ceil.GetType()) {
        case WarehouseCeilType::start:
          start_ceils_.push_back({row_ind, col_ind});
          break;
        case WarehouseCeilType::finish:
          finish_ceils_.push_back({row_ind, col_ind});
          break;
        default:
          break;
      }
      ++col_ind;
    }
    ++row_ind;
  }

  if (start_ceils_.empty() || finish_ceils_.empty()) {
    throw std::runtime_error("Invalid map. At least one T and Y was expected");
  }
}

Warehouse Warehouse::WarehouseFromFile(std::string_view filepath) {
  std::ifstream fin(filepath.data());
  if (fin.fail()) {
    throw std::runtime_error("Error opening the file " + std::string(filepath));
  }

  std::vector<std::vector<std::string>> rows;
  while (fin.good()) {
    std::vector<std::string> row;
    std::string string_row;
    std::getline(fin, string_row);

    std::stringstream ss(string_row);
    while (ss.good()) {
      std::string substr;
      getline(ss, substr, ' ');
      row.emplace_back(std::move(substr));
    }
    rows.emplace_back(std::move(row));
  }

  return Warehouse(rows);
}

uint32_t Warehouse::Height() const { return warehouse_.size(); }

uint32_t Warehouse::Width() const { return warehouse_[0].size(); }

const std::vector<Coordinates> &Warehouse::GetStartVertices() const & {
  return start_ceils_;
}

const std::vector<Coordinates> &Warehouse::GetFinishVertices() const & {
  return finish_ceils_;
}

size_t Warehouse::NumberStartVertices() const {
  return GetStartVertices().size();
}

size_t Warehouse::NumberFinishVertices() const {
  return GetFinishVertices().size();
}

std::vector<Coordinates> Warehouse::GetNeighbours(Coordinates coord) const {
  if (!warehouse_[coord.x][coord.y].IsAvailable()) {
    return {};
  }

  std::vector<Coordinates> neighbours;
  if (coord.y != Width() - 1 &&
      warehouse_[coord.x][coord.y + 1].IsAvailable()) {
    neighbours.push_back({coord.x, coord.y + 1});
  }
  if (coord.y != 0 && warehouse_[coord.x][coord.y - 1].IsAvailable()) {
    neighbours.push_back({coord.x, coord.y - 1});
  }
  if (coord.x != Height() - 1 &&
      warehouse_[coord.x + 1][coord.y].IsAvailable()) {
    neighbours.push_back({coord.x + 1, coord.y});
  }
  if (coord.x != 0 && warehouse_[coord.x - 1][coord.y].IsAvailable()) {
    neighbours.push_back({coord.x - 1, coord.y});
  }
  return neighbours;
}

}  // namespace aco