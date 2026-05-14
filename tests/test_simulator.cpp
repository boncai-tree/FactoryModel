#include <gtest/gtest.h>
#include <sstream>
#include <string_view>

#include "Parser.h"
#include "Simulator.h"
#include "test_helpers.hpp"

using workshop::parse;
using workshop::simulate;

TEST(SimulatorTest, NoItemsOutputsStopZero) {
  std::istringstream pin(
      "2 1\n"
      "5\n"
      "0\n");
  auto data = parse(pin);
  std::ostringstream out;
  simulate(data, out);
  EXPECT_EQ(normalizeNewlines(out.str()), "stop 0\n");
}

TEST(SimulatorTest, MatchesGoldenExample1Consistent) {
  std::istringstream pin(readWholeFile(examplesPath() / "example1.txt"));
  auto data = parse(pin);
  std::ostringstream actual;
  simulate(data, actual);

  static constexpr std::string_view kExpected =
      "start 0 0 0 0\n"
      "start 0 3 1 1\n"
      "finish 3 0 0 0\n"
      "start 3 1 1 0\n"
      "wait 3 0 1 1 1\n"
      "finish 6 3 1 1\n"
      "start 6 4 0 1\n"
      "ready 6 3 1\n"
      "finish 7 1 1 0\n"
      "start 7 2 0 0\n"
      "ready 7 1 0\n"
      "finish 10 2 0 0\n"
      "start 10 2 1 0\n"
      "wait 10 2 1 0 0\n"
      "finish 11 4 0 1\n"
      "start 11 0 1 1\n"
      "wait 11 4 1 0 0\n"
      "finish 14 2 1 0\n"
      "start 14 4 1 0\n"
      "ready 14 2 0\n"
      "finish 17 0 1 1\n"
      "ready 17 0 1\n"
      "finish 18 4 1 0\n"
      "ready 18 4 0\n"
      "stop 18\n";

  EXPECT_EQ(normalizeNewlines(actual.str()), normalizeNewlines(kExpected));
}

TEST(SimulatorTest, MatchesGoldenM2N2TwoItems) {
  std::istringstream pin(readWholeFile(examplesPath() / "m2n2_two_items.txt"));
  auto data = parse(pin);
  std::ostringstream actual;
  simulate(data, actual);

  static constexpr std::string_view kExpected =
      "start 0 0 0 0\n"
      "start 0 1 0 1\n"
      "finish 3 0 0 0\n"
      "ready 3 0 0\n"
      "finish 5 1 0 1\n"
      "ready 5 1 1\n"
      "stop 5\n";

  EXPECT_EQ(normalizeNewlines(actual.str()), normalizeNewlines(kExpected));
}

TEST(SimulatorTest, MatchesGoldenM3N1Pipeline) {
  std::istringstream pin(readWholeFile(examplesPath() / "m3n1_pipeline.txt"));
  auto data = parse(pin);
  std::ostringstream actual;
  simulate(data, actual);

  static constexpr std::string_view kExpected =
      "start 0 0 0 0\n"
      "finish 7 0 0 0\n"
      "start 7 1 0 0\n"
      "wait 7 0 1 0 2\n"
      "finish 14 1 0 0\n"
      "start 14 2 0 0\n"
      "wait 14 1 1 0 2\n"
      "finish 21 2 0 0\n"
      "start 21 0 1 0\n"
      "wait 21 2 1 0 2\n"
      "finish 32 0 1 0\n"
      "start 32 1 1 0\n"
      "ready 32 0 0\n"
      "finish 43 1 1 0\n"
      "start 43 2 1 0\n"
      "ready 43 1 0\n"
      "finish 54 2 1 0\n"
      "ready 54 2 0\n"
      "stop 54\n";

  EXPECT_EQ(normalizeNewlines(actual.str()), normalizeNewlines(kExpected));
}

TEST(SimulatorTest, MatchesGoldenM3N3ThreeMachines) {
  std::istringstream pin(readWholeFile(examplesPath() / "m3n3_three_machines.txt"));
  auto data = parse(pin);
  std::ostringstream actual;
  simulate(data, actual);

  static constexpr std::string_view kExpected =
      "start 0 0 0 0\n"
      "start 0 3 1 1\n"
      "start 0 6 0 2\n"
      "finish 3 0 0 0\n"
      "start 3 1 1 0\n"
      "wait 3 0 1 0 2\n"
      "finish 7 6 0 2\n"
      "start 7 7 0 2\n"
      "wait 7 6 1 0 2\n"
      "finish 13 3 1 1\n"
      "start 13 4 0 1\n"
      "ready 13 3 1\n"
      "finish 14 1 1 0\n"
      "finish 14 7 0 2\n"
      "start 14 2 0 0\n"
      "start 14 8 1 2\n"
      "wait 14 7 1 1 1\n"
      "ready 14 1 0\n"
      "finish 17 2 0 0\n"
      "start 17 0 1 0\n"
      "wait 17 2 1 2 0\n"
      "finish 18 4 0 1\n"
      "start 18 5 1 1\n"
      "wait 18 4 1 0 1\n"
      "finish 28 0 1 0\n"
      "start 28 6 1 0\n"
      "ready 28 0 0\n"
      "finish 31 8 1 2\n"
      "finish 31 5 1 1\n"
      "start 31 2 1 2\n"
      "start 31 7 1 1\n"
      "ready 31 8 2\n"
      "ready 31 5 1\n"
      "finish 39 6 1 0\n"
      "start 39 4 1 0\n"
      "ready 39 6 0\n"
      "finish 44 7 1 1\n"
      "ready 44 7 1\n"
      "finish 48 2 1 2\n"
      "ready 48 2 2\n"
      "finish 50 4 1 0\n"
      "ready 50 4 0\n"
      "stop 50\n";

  EXPECT_EQ(normalizeNewlines(actual.str()), normalizeNewlines(kExpected));
}

TEST(SimulatorTest, MatchesGoldenExample3OneMachine) {
  std::istringstream pin(readWholeFile(examplesPath() / "example3_one_machine.txt"));
  auto data = parse(pin);
  std::ostringstream actual;
  simulate(data, actual);

  static constexpr std::string_view kExpected =
      "start 0 0 0 0\n"
      "finish 2 0 0 0\n"
      "start 2 1 1 0\n"
      "wait 2 0 1 0 2\n"
      "finish 5 1 1 0\n"
      "start 5 2 2 0\n"
      "wait 5 1 2 0 2\n"
      "finish 6 2 2 0\n"
      "start 6 0 1 0\n"
      "ready 6 2 0\n"
      "finish 9 0 1 0\n"
      "start 9 1 2 0\n"
      "wait 9 0 2 0 1\n"
      "finish 10 1 2 0\n"
      "start 10 0 2 0\n"
      "ready 10 1 0\n"
      "finish 11 0 2 0\n"
      "ready 11 0 0\n"
      "stop 11\n";

  EXPECT_EQ(normalizeNewlines(actual.str()), normalizeNewlines(kExpected));
}

TEST(SimulatorTest, Example4M1StopZero) {
  std::istringstream pin(readWholeFile(examplesPath() / "example4_m1.txt"));
  auto data = parse(pin);
  std::ostringstream out;
  simulate(data, out);
  EXPECT_EQ(normalizeNewlines(out.str()), "stop 0\n");
}
