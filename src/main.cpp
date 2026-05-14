#include "Parser.h"
#include "Simulator.h"

#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  if (argc < 2) {
    std::cerr << "Usage: " << (argc > 0 ? argv[0] : "workshop") << " <input_file>" << std::endl;
    return 1;
  }

  workshop::InputData data;
  {
    std::ifstream fin(argv[1]);
    if (!fin.is_open()) {
      std::cerr << "Cannot open input file: " << argv[1] << std::endl;
      return 1;
    }
    try {
      data = workshop::parse(fin);
    } catch (workshop::ParseError const& e) {
      std::cout << e.what() << std::endl;
      return 1;
    }
  }

  workshop::simulate(data, std::cout);
  return 0;
}
