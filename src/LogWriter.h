#pragma once

#include <array>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>

namespace workshop {

class LogWriter {
public:
  static constexpr std::size_t kFlushThreshold = 1u << 16;

  explicit LogWriter(std::ostream& out)
      : out_(out) {
    buf_.reserve(kFlushThreshold * 2);
  }

  ~LogWriter() noexcept {
    try {
      flush();
    } catch (...) {
      std::cerr << "Couldn't flush log writer in destructor" << std::endl;
    }
  }

  LogWriter(LogWriter const&) = delete;
  LogWriter& operator=(LogWriter const&) = delete;

  void flush() {
    if (!buf_.empty()) {
      out_.write(buf_.data(), static_cast<std::streamsize>(buf_.size()));
      buf_.clear();
    }
  }

  void append(std::string_view sv) {
    buf_.insert(buf_.end(), sv.begin(), sv.end());
    if (buf_.size() >= kFlushThreshold) {
      flush();
    }
  }

  void append(char c) {
    append(std::string_view(&c, 1));
  }

  template <std::integral T>
    requires (!std::same_as<T, char>)
  void append(T v) {
    std::array<char, 20> tmp{};
    auto const [ptr, ec] = std::to_chars(tmp.data(), tmp.end(), v);
    append(std::string_view(tmp.data(), static_cast<std::size_t>(ptr - tmp.data())));
  }

  template <typename... Args>
  void writeLine(std::string_view keyword, Args&&... args) {
    append(keyword);
    if constexpr (sizeof...(Args) > 0) {
      ((append(' '), append(std::forward<Args>(args))), ...);
    }
    append('\n');
  }

private:
  std::ostream& out_;
  std::string buf_;
};

} // namespace workshop
