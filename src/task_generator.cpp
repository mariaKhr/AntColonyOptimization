#include <fstream>
#include <task_generator.hpp>

namespace aco {

std::vector<double> ReadWeights(std::string_view filepath) {
  std::ifstream fin(filepath.data());
  if (fin.fail()) {
    throw std::runtime_error("Error opening the file " + std::string(filepath));
  }

  std::vector<double> weights;
  double value;
  while (fin >> value) {
    weights.push_back(value);
  }

  return weights;
}

std::vector<uint32_t> CreateTargets(size_t size) {
  std::vector<uint32_t> targets(size);
  std::iota(targets.begin(), targets.end(), 0);
  return targets;
}

} // namespace aco