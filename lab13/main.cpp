#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int step(float probability, int flips) {
    int heads = 0;
    for (int i = 0; i < flips; ++i)
        if (rand() < probability * RAND_MAX)
            heads++;

    return heads;
}

int run(float probability, int flips) {
    int sum = 0;
    for(int i = 0; i<10; i++) {
        sum += step(probability, flips);
    }
    return sum;
}

int main() {
    srand(time(nullptr));
    int m = 10;
    int flip_counts[] = {100, 1000, 10000, 100000, 1000000};
    float probabilities[] = {0.1, 0.3, 0.5, 0.7, 0.9};
    float actual_rates[5][5];

    for (int p = 0; p < 5; p++) {
        for (int n = 0; n < 5; n++) {
            int heads = run(probabilities[p], flip_counts[n]);
            actual_rates[p][n] = (float) heads / (m * flip_counts[n]);
        }
    }

    cout << "Each cell shows actual heads rate (actual - expected)\n";
    cout << "p/n";
    for (int n = 0; n < 5; n++)
        cout << "\t" << flip_counts[n];
    cout << "\n";

    for (int p = 0; p < 5; p++) {
        cout << probabilities[p];
        for (int n = 0; n < 5; n++) {
            cout << "\t" << actual_rates[p][n] << " ("
                 << actual_rates[p][n] - probabilities[p] << ")";
        }
        cout << "\n";
    }

    return 0;
}
