#include "random_walk.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <cstdlib>
#include <fstream>
#include <string>

namespace {
class CsvBenchmarkListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override {
        output.open("random_walk_benchmark.csv");
        output << "steps,trials,runtime_ns\n";
    }

    void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
        output << stats.info.name << "," << stats.mean.point.count() << "\n";
    }

private:
    std::ofstream output;
};
}

CATCH_REGISTER_LISTENER(CsvBenchmarkListener)

TEST_CASE("random walk benchmark") {
    constexpr int trials = 10;
    std::ofstream output("random_walk.csv");
    REQUIRE(output);
    output << "steps,trials,trial,step,position\n";

    for (int steps = 5; steps < 10; ++steps) {
        for (int trial = 1; trial <= trials; ++trial) {
            int position = 0;
            output << steps << "," << trials << "," << trial << ",0,0\n";
            for (int step = 1; step <= steps; ++step) {
                position += std::rand() % 2 == 0 ? 1 : -1;
                output << steps << "," << trials << "," << trial << ","
                       << step << "," << position << "\n";
            }
        }

        BENCHMARK(std::to_string(steps) + "," + std::to_string(trials)) {
            int finalPosition = 0;
            for (int trial = 0; trial < trials; ++trial) {
                finalPosition += randomWalk(steps);
            }
            return finalPosition;
        };
    }
}
