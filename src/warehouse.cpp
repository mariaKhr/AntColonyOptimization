#include <fstream>
#include <sstream>
#include <warehouse.hpp>

namespace aco {

WarehouseCeil::WarehouseCeil(std::string_view type) {
  if (type.compare("!") == 0) {
    type_ = WarehouseCeilType::unavailable;
  } else if (type.compare(".") == 0) {
    type_ = WarehouseCeilType::empty;
  } else if (type.compare("S") == 0) {
    type_ = WarehouseCeilType::start;
  } else if (type.compare("F") == 0) {
    type_ = WarehouseCeilType::finish;
  } else {
    throw std::runtime_error("Invalid ceil type");
  }
}

WarehouseCeilType WarehouseCeil::GetType() const { return type_; }

bool WarehouseCeil::IsAvailable() const {
  return type_ != WarehouseCeilType::unavailable;
}

Warehouse::Warehouse(const std::vector<std::vector<std::string>> &map) {
  size_t height = map.size();
  size_t width = 0;

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
  for (size_t row_ind = 0, rows = map.size(); row_ind < rows; ++row_ind) {
    const auto &row = map[row_ind];
    for (size_t col_ind = 0, cols = row.size(); col_ind < cols; ++col_ind) {
      const auto &new_ceil = warehouse_[row_ind].emplace_back(row[col_ind]);
      switch (new_ceil.GetType()) {
      case WarehouseCeilType::start:
        start_ceils_.emplace_back(row_ind, col_ind);
        break;
      case WarehouseCeilType::finish:
        finish_ceils_.emplace_back(row_ind, col_ind);
        break;
      default:
        break;
      }
    }
  }

  if (start_ceils_.empty() || finish_ceils_.empty()) {
    throw std::runtime_error(
        "Invalid map. At least one Start and Finish ceil was expected");
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
      row.push_back(std::move(substr));
    }
    rows.push_back(std::move(row));
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
  neighbours.reserve(4);
  if (coord.y != Width() - 1 &&
      warehouse_[coord.x][coord.y + 1].IsAvailable()) {
    neighbours.emplace_back(coord.x, coord.y + 1);
  }
  if (coord.y != 0 && warehouse_[coord.x][coord.y - 1].IsAvailable()) {
    neighbours.emplace_back(coord.x, coord.y - 1);
  }
  if (coord.x != Height() - 1 &&
      warehouse_[coord.x + 1][coord.y].IsAvailable()) {
    neighbours.emplace_back(coord.x + 1, coord.y);
  }
  if (coord.x != 0 && warehouse_[coord.x - 1][coord.y].IsAvailable()) {
    neighbours.emplace_back(coord.x - 1, coord.y);
  }
  return neighbours;
}

} // namespace aco