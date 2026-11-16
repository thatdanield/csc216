#include <iostream>

int step(float probability, int flips) {
    srand(time(nullptr));
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
    int m = 10;
    int flip_counts[] = {100, 1000, 10000, 100000, 1000000};
    float probabilities[] = {0.1, 0.3, 0.5, 0.7, 0.9};

    std::cout << "n p expected actual\n";

    for (int i = 0; i < 5; i++) {
        int n = flip_counts[i];
        float p = probabilities[i];
        int heads = run(p, n);
        float actual_rate = (float) heads / (m * n);
        std::cout << n << " " << p << " " << p << " " << actual_rate << "\n";
    }

    return 0;
}