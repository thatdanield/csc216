#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <fstream>
#include <string>

int naivePower(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (base == 0) {
        return 0;
    }
    return base * naivePower(base, exponent - 1);
}

int unoptimizedDCPower(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (base == 0) {
        return 0;
    }
    if (exponent % 2 == 0) {
        return unoptimizedDCPower(base * base, exponent / 2);
    }
    return base * unoptimizedDCPower(base * base, (exponent - 1) / 2);
}

int optimizedDCPower(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (base == 0) {
        return 0;
    }
    const int halfPower = optimizedDCPower(base, exponent / 2);
    if (exponent % 2 == 0) {
        return halfPower * halfPower;
    }
    return base * halfPower * halfPower;
}

template <typename Function>
long long runtimeInNanoseconds(Function&& function) {
    constexpr int iterations = 100'000;
    volatile int result = 0;
    const auto start = std::chrono::steady_clock::now();
    for (int iteration = 0; iteration < iterations; ++iteration) {
        result = function();
    }
    const auto elapsed = std::chrono::steady_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count()
           / iterations;
}

TEST_CASE("power algorithms produce expected results and runtime CSV") {
    std::ofstream output("power_benchmark.csv");
    REQUIRE(output);
    output << "x,n,naive_runtime_ns,unoptimized_dc_runtime_ns,optimized_dc_runtime_ns\n";

    for (int x = 0; x <= 8; ++x) {
        for (int n = 0; n <= 8; ++n) {
            if (x == 8 && n == 8) {
                continue;
            }

            int expected = 1;
            for (int i = 0; i < n; ++i) {
                expected *= x;
            }

            REQUIRE(naivePower(x, n) == expected);
            REQUIRE(unoptimizedDCPower(x, n) == expected);
            REQUIRE(optimizedDCPower(x, n) == expected);

            const auto naiveRuntime =
                runtimeInNanoseconds([&] { return naivePower(x, n); });
            const auto unoptimizedDCRuntime =
                runtimeInNanoseconds([&] { return unoptimizedDCPower(x, n); });
            const auto optimizedDCRuntime =
                runtimeInNanoseconds([&] { return optimizedDCPower(x, n); });

            output << x << "," << n << "," << naiveRuntime << ","
                   << unoptimizedDCRuntime << "," << optimizedDCRuntime << "\n";
        }
    }
}
