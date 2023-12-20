#include <exception>
#include <iostream>

#include "ACO.hpp"
#include "warehouse.hpp"

using namespace aco;

int main(int argc, char *argv[]) {
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
        .start_vertex = static_cast<uint32_t>(warehouse.GetStartVertex()),
        .finish_vertex =
            static_cast<uint32_t>(*warehouse.GetFinishVertexes().begin()),
    };

    auto ACO = BasicACO(std::move(parameters), graph);
    Route route;
    for (uint32_t i = 0; i < 1000; ++i) {
      route = ACO.Execute();
    }
    std::cout << "Found route:\n";
    warehouse.VisualizeRoute(std::cout, route);

  } catch (const std::exception &ex) {
    std::cout << ex.what() << "\n";
    return 1;
  }
}