#include <exception>
#include <iostream>

#include "ACO.hpp"
#include "warehouse.hpp"

using namespace aco;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Invalid command line argument. Expected path to csv file\n";
    return 1;
  }

  try {
    auto warehouse_filepath = argv[1];
    auto warehouse_content = ReadCSVFile(warehouse_filepath);
    auto warehouse = Warehouse(warehouse_content);
    std::cout << "Warehouse map:\n" << warehouse;

    auto graph = Warehouse::ToGraph(warehouse);

    ACOParameters parameters{
        .ants = 100,
        .num_iter = 10,
        .alpha = 0.5,
        .beta = 0.8,
        .q = 10,
        .ro = 0.2,
        .taumax = 2,
        .start_vertex = static_cast<uint32_t>(warehouse.GetStartVertex()),
        .finish_vertex =
            static_cast<uint32_t>(warehouse.GetFinishVertexes()[0]),
    };

    auto ACO =
        std::make_unique<BasicACO>(std::move(parameters), std::move(graph));
    auto best_route = ACO->Execute();

    std::cout << "Best route:\n";
    warehouse.VisualizeRoute(std::cout, best_route);

  } catch (const std::exception& ex) {
    std::cout << ex.what() << "\n";
    return 1;
  }
}