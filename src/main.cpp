#include <exception>
#include <iostream>

#include "model.hpp"

using namespace aco;

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      throw std::runtime_error(
          "Invalid command line arguments. Expected path to file with "
          "warehouse and path to file with tasks probabilities\n");
    }

    const auto warehouse = Warehouse::WarehouseFromFile(argv[1]);
    const auto weights = ReadWeights(argv[2]);

    auto model = Model(warehouse, weights);
    model.Run();
    std::cout << "Time: " << model.GetTime() << "\n";

  } catch (const std::exception& ex) {
    std::cout << ex.what() << "\n";
    return 1;
  }
}