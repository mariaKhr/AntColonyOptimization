#include <catch2/catch.hpp>
#include <unordered_set>

#include "warehouse.hpp"

using namespace aco;

TEST_CASE("TestWarehouse") {
  Rows rows{{"Y", "R", "T"}, {"G", "G", "Y"}};
  Warehouse warehouse(rows);

  REQUIRE(warehouse.GetStartVertex() == 2);
  std::unordered_set<size_t> expected_finish_vertexes = {0, 5};
  REQUIRE(warehouse.GetFinishVertexes() == expected_finish_vertexes);
}

TEST_CASE("TestWarehouseToGraph") {
  Rows rows{{"Y", "R", "T"}, {"G", "G", "Y"}};
  Warehouse warehouse(rows);

  auto graph = Warehouse::ToGraph(warehouse);
  REQUIRE(graph.Size() == 6);

  std::unordered_map<size_t, std::unordered_set<size_t>> expected_edges = {
      {0, {3}}, {1, {}}, {2, {5}}, {3, {0, 4}}, {4, {3, 5}}, {5, {2, 4}}};
  for (size_t from = 0; from < 6; ++from) {
    for (size_t to = 0; to < 6; ++to) {
      REQUIRE(graph.HasEdge(from, to) == expected_edges[from].contains(to));
    }
  }

  for (size_t from = 0; from < 6; ++from) {
    for (size_t to = 0; to < 6; ++to) {
      if (graph.HasEdge(from, to)) {
        REQUIRE(graph.GetEdge(from, to) == 1);
      }
    }
  }
}