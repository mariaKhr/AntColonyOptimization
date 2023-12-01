#include "csv.hpp"

#include <fstream>
#include <sstream>

namespace aco {

Row ReadLineCSV(std::istream &istream) {
  std::string string_row;
  std::getline(istream, string_row);

  Row row;

  std::stringstream ss(string_row);
  while (ss.good()) {
    std::string substr;
    getline(ss, substr, ',');
    row.emplace_back(std::move(substr));
  }

  return row;
}

Rows ReadLinesCSV(std::istream &istream) {
  Rows rows;

  while (istream.good()) {
    auto row = ReadLineCSV(istream);
    rows.emplace_back(std::move(row));
  }

  return rows;
}

Rows ReadCSVFile(std::string_view file_name) {
  std::ifstream fin(file_name.data());
  if (fin.fail()) {
    throw std::runtime_error("Error opening the file " +
                             std::string(file_name));
  }
  return ReadLinesCSV(fin);
}

}  // namespace aco