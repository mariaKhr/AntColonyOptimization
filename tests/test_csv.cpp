#include <catch2/catch.hpp>
#include <fstream>

#include "csv.hpp"

using namespace aco;

TEST_CASE("TestReadLineCSV") {
  std::stringstream stream("G,R,T\n");
  auto row = ReadLineCSV(stream);
  Row expected = {"G", "R", "T"};
  REQUIRE(row == expected);
}

TEST_CASE("TestReadLinesCSV") {
  std::stringstream stream("G,R,T\nG,G,Y");
  auto rows = ReadLinesCSV(stream);
  Rows expected = {{"G", "R", "T"}, {"G", "G", "Y"}};
  REQUIRE(rows == expected);
}

TEST_CASE("TestReadCSVFile") {
  const auto path = "test.csv";
  unlink(path);
  std::ofstream file(path, std::fstream::out | std::fstream::trunc);
  file << "G,R,T\nG,G,Y";
  file.close();

  auto rows = ReadCSVFile(path);
  Rows expected = {{"G", "R", "T"}, {"G", "G", "Y"}};
  REQUIRE(rows == expected);
}
