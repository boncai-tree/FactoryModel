#pragma once

#include <istream>
#include <stdexcept>
#include <string>
#include <vector>

namespace workshop {

struct InputData {
  int M = 0;
  int N = 0;
  std::vector<std::vector<int>> T;
  std::vector<std::vector<int>> queues;
};

class ParseError final : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

InputData parse(std::istream& in);

} // namespace workshop
