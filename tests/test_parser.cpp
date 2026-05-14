#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string_view>

#include "Parser.h"
#include "test_helpers.hpp"

using workshop::InputData;
using workshop::ParseError;
using workshop::parse;

TEST(ParserTest, Example1ConsistentDimensions) {
  std::ifstream in(examplesPath() / "example1.txt");
  ASSERT_TRUE(in.is_open());
  InputData d = parse(in);
  EXPECT_EQ(d.M, 3);
  EXPECT_EQ(d.N, 2);
  EXPECT_EQ(d.T.size(), 2u);
  EXPECT_EQ(d.T[0].size(), 2u);
  EXPECT_EQ(d.T[0][0], 3);
  EXPECT_EQ(d.T[0][1], 5);
  EXPECT_EQ(d.T[1][0], 4);
  EXPECT_EQ(d.T[1][1], 6);
  EXPECT_EQ(d.queues.size(), 2u);
  EXPECT_EQ(d.queues[0].size(), 3u);
  EXPECT_EQ(d.queues[0], (std::vector<int>{0, 1, 0}));
  EXPECT_EQ(d.queues[1].size(), 2u);
  EXPECT_EQ(d.queues[1], (std::vector<int>{1, 0}));
}

TEST(ParserTest, Mc1EmptyQueues) {
  std::ifstream in(examplesPath() / "example4_m1.txt");
  ASSERT_TRUE(in.is_open());
  InputData d = parse(in);
  EXPECT_EQ(d.M, 1);
  EXPECT_EQ(d.N, 1);
  EXPECT_TRUE(d.T.empty());
  EXPECT_EQ(d.queues.size(), 1u);
  for (auto const& q : d.queues) {
    EXPECT_TRUE(q.empty());
  }
}

TEST(ParserTest, BadRangeThrows) {
  std::ifstream in(examplesPath() / "bad_range.txt");
  ASSERT_TRUE(in.is_open());
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, BadTypeThrows) {
  std::ifstream in(examplesPath() / "bad_type.txt");
  ASSERT_TRUE(in.is_open());
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, BadCountThrows) {
  std::ifstream in(examplesPath() / "bad_count.txt");
  ASSERT_TRUE(in.is_open());
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, BadLetterThrows) {
  std::ifstream in(examplesPath() / "bad_letter.txt");
  ASSERT_TRUE(in.is_open());
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, PrematureEofThrows) {
  std::istringstream in("3 2\n");
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, EmptyInputThrows) {
  std::istringstream in("");
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, ZeroDimensionsThrows) {
  std::istringstream in("0 3\n");
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, TrailingGarbageThrows) {
  std::istringstream in(
      "2 1\n"
      "5\n"
      "0\n"
      "garbage\n");
  EXPECT_THROW(parse(in), ParseError);
}

TEST(ParserTest, TrailingWhitespaceLinesOk) {
  std::istringstream in(
      "2 1\n"
      "5\n"
      "0\n"
      "   \n"
      "\n");
  EXPECT_NO_THROW(parse(in));
}
