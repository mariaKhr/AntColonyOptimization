#pragma once

#include <string_view>
#include <vector>

namespace aco {

using Row = std::vector<std::string>;
using Rows = std::vector<Row>;

Row ReadLineCSV(std::istream &istream);
Rows ReadLinesCSV(std::istream &istream);
Rows ReadCSVFile(std::string_view file_name);

}  // namespace aco