#include "Parser.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <istream>
#include <string>
#include <string_view>

namespace workshop {

namespace {

constexpr int kMaxMN = 100;
constexpr int kMaxTime = 10'000;
constexpr int kMaxQueueLen = 100'000;

class LineParser {
public:
  explicit LineParser(std::string& line) noexcept
      : line_(line)
      , sv_(line) {}

  int next(int upperBound) {
    assert(upperBound >= 0);
    skipWhitespace();
    unsigned long long v = 0;
    auto const [ptr, ec] = std::from_chars(sv_.data(), sv_.end(), v);
    if (ec != std::errc{} || v > static_cast<unsigned long long>(upperBound)) {
      throw ParseError(line_);
    }
    sv_.remove_prefix(ptr - sv_.data());
    return static_cast<int>(v);
  }

  void expectEnd() {
    if (!std::ranges::all_of(sv_, [](char c) { return std::isspace(static_cast<unsigned char>(c)); })) {
      throw ParseError(line_);
    }
  }

private:
  std::string& line_;
  std::string_view sv_;

  void skipWhitespace() noexcept {
    while (!sv_.empty() && std::isspace(static_cast<unsigned char>(sv_.front()))) {
      sv_.remove_prefix(1);
    }
  }
};

} // namespace

InputData parse(std::istream& in) {
  InputData data;
  std::string line;

  auto const readNext = [&in, &line]() {
    if (!std::getline(in, line)) {
      throw ParseError("");
    }
  };

  readNext();
  {
    LineParser lp(line);
    int const m = lp.next(kMaxMN);
    int const n = lp.next(kMaxMN);
    lp.expectEnd();
    if (m < 1 || n < 1) {
      throw ParseError(line);
    }
    data.M = m;
    data.N = n;
  }

  data.T.assign(data.M - 1, std::vector<int>(data.N));
  for (int i = 0; i < data.M - 1; ++i) {
    readNext();
    LineParser lp(line);
    for (int j = 0; j < data.N; ++j) {
      data.T[i][j] = lp.next(kMaxTime);
    }
    lp.expectEnd();
  }

  data.queues.resize(data.N);
  for (int j = 0; j < data.N; ++j) {
    readNext();
    LineParser lp(line);

    int q = lp.next(kMaxQueueLen);
    data.queues[j].resize(static_cast<std::size_t>(q));

    int maxType = data.M - 2;
    if (maxType < 0 && q != 0) {
      throw ParseError(line);
    }
    for (int& p : data.queues[j]) {
      p = lp.next(maxType);
    }
    lp.expectEnd();
  }

  while (std::getline(in, line)) {
    LineParser(line).expectEnd();
  }

  return data;
}

} // namespace workshop
