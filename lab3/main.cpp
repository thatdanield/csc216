#include <iostream>
#include <fstream>
#include <chrono>

int memo[100];

int naiveG(int n) {
    if (n == 0)
        return 0;
    return n - naiveG(naiveG(n - 1));
}

int memoizedG(int n) {
    if (n == 0)
        return 0;

    if (memo[n])
        return memo[n];

    memo[n] = n - memoizedG(memoizedG(n - 1));
    return memo[n];
}

int main() {
    std::ofstream file("timing.csv");

    file << "n,naive,memoized\n";

    for (int n = 0; n <= 30; n++) {

        // Time naiveG
        auto start = std::chrono::high_resolution_clock::now();

        naiveG(n);

        auto end = std::chrono::high_resolution_clock::now();

        auto naiveTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                end - start
            ).count();


        // Reset memoization array
        for (int i = 0; i < 100; i++)
            memo[i] = 0;


        // Time memoizedG
        start = std::chrono::high_resolution_clock::now();

        memoizedG(n);

        end = std::chrono::high_resolution_clock::now();

        auto memoizedTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                end - start
            ).count();


        file << n << "," << naiveTime << "," << memoizedTime << "\n";
    }

    file.close();
}