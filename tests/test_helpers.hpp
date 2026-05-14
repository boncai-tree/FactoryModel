#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "test_paths.hpp"

inline std::string readWholeFile(std::filesystem::path const& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    return {};
  }
  std::ostringstream oss;
  oss << in.rdbuf();
  return oss.str();
}

inline std::string normalizeNewlines(std::string s) {
  s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
  return s;
}

inline std::string normalizeNewlines(std::string_view s) {
  return normalizeNewlines(std::string(s));
}

inline std::filesystem::path examplesPath() { return kWorkshopExamplesDir; }
