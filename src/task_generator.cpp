#include "task_generator.hpp"

#include <fstream>

namespace aco {

std::vector<double> ReadWeights(std::string_view filepath) {
  std::ifstream fin(filepath.data());
  if (fin.fail()) {
    throw std::runtime_error("Error opening the file " + std::string(filepath));
  }

  std::vector<double> rows;
  double value;
  while (fin.good()) {
    fin >> value;
    rows.push_back(value);
  }

  return rows;
}

std::vector<uint32_t> CreateTargets(size_t size) {
  std::vector<uint32_t> targets(size);
  std::iota(targets.begin(), targets.end(), 0);
  return targets;
}

}  // namespace aco