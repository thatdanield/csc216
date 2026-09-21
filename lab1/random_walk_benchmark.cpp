#include "random_walk.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <cstdio>
#include <fstream>
#include <string>

namespace {
class CsvBenchmarkListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override {
        output.open("random_walk.csv");
        output << "steps,trials,runtime_ns\n";
    }

    void benchmarkEnded(Catch::BenchmarkStats<> const& stats) override {
        int steps;
        int trials;
        std::sscanf(stats.info.name.c_str(), "%d,%d", &steps, &trials);
        output << steps << "," << trials << "," << stats.mean.point.count() << "\n";
    }

private:
    std::ofstream output;
};
}

CATCH_REGISTER_LISTENER(CsvBenchmarkListener)

TEST_CASE("random walk benchmark") {
    constexpr int trials = 10;
    for (int steps = 5; steps < 10; ++steps) {
        BENCHMARK(std::to_string(steps) + "," + std::to_string(trials)) {
            int finalPosition = 0;
            for (int trial = 0; trial < trials; ++trial) {
                finalPosition += randomWalk(steps);
            }
            return finalPosition;
        };
    }
}
