#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#define main benchmark_indexer_main
#include "main.cpp"
#undef main

namespace {

class DiscardOutput : public std::streambuf {
protected:
    int_type overflow(int_type character) override {
        return traits_type::not_eof(character);
    }
};

class TemporaryInputFile {
public:
    explicit TemporaryInputFile(const std::string& contents) {
        static int nextId = 0;
        path = std::filesystem::temp_directory_path()
                / ("lab10-index-benchmark-"
                   + std::to_string(std::chrono::steady_clock::now()
                                            .time_since_epoch()
                                            .count())
                   + "-" + std::to_string(nextId++) + ".txt");

        std::ofstream file(path);
        if (!file || !(file << contents)) {
            throw std::runtime_error("Could not create benchmark input file");
        }
    }

    ~TemporaryInputFile() {
        std::error_code error;
        std::filesystem::remove(path, error);
        std::filesystem::remove(path.string() + ".index", error);
    }

    const std::filesystem::path& getPath() const {
        return path;
    }

private:
    std::filesystem::path path;
};

int runIndexer(const std::filesystem::path& path) {
    std::istringstream input(path.string() + '\n');
    DiscardOutput discardOutput;
    std::streambuf* oldInput = std::cin.rdbuf(input.rdbuf());
    std::streambuf* oldOutput = std::cout.rdbuf(&discardOutput);

    int result = benchmark_indexer_main();

    std::cin.clear();
    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);
    return result;
}

std::string essayWithWordCount(const std::vector<std::string>& essayWords,
                               std::size_t wordCount) {
    std::string text;
    for (std::size_t i = 0; i < wordCount; i++) {
        if (!text.empty()) {
            text += ' ';
        }
        text += essayWords[i % essayWords.size()];
    }
    return text;
}

} // namespace

TEST_CASE("benchmark indexing runtime across document sizes") {
    std::ifstream essayFile(ESSAY_FILE);
    REQUIRE(essayFile.is_open());

    std::vector<std::string> essayWords;
    std::string word;
    while (essayFile >> word) {
        essayWords.push_back(word);
    }
    REQUIRE_FALSE(essayWords.empty());

    const std::vector<std::pair<std::string, std::size_t>> cases = {
        {"sentence", 12},
        {"paragraph", 100},
        {"short essay", 1000},
        {"The American Scholar", essayWords.size()},
        {"long essay", 25000},
        {"very long essay", 100000}
    };

    std::ofstream csv(BENCHMARK_CSV);
    REQUIRE(csv.is_open());
    csv << "document,words,mean_ms,median_ms,min_ms\n";
    csv << std::fixed << std::setprecision(3);

    for (const auto& testCase : cases) {
        const std::string contents = testCase.first == "sentence"
                ? "A reader studies words and records useful ideas from an essay today."
                : essayWithWordCount(essayWords, testCase.second);
        TemporaryInputFile inputFile(contents);

        REQUIRE(runIndexer(inputFile.getPath()) == 0);

        std::vector<double> milliseconds;
        for (int repeat = 0; repeat < 5; repeat++) {
            const auto start = std::chrono::steady_clock::now();
            REQUIRE(runIndexer(inputFile.getPath()) == 0);
            const auto finish = std::chrono::steady_clock::now();
            milliseconds.push_back(
                    std::chrono::duration<double, std::milli>(finish - start).count());
        }

        std::sort(milliseconds.begin(), milliseconds.end());
        const double mean = std::accumulate(milliseconds.begin(), milliseconds.end(), 0.0)
                / milliseconds.size();
        csv << testCase.first << ',' << testCase.second << ',' << mean << ','
            << milliseconds[milliseconds.size() / 2] << ',' << milliseconds.front() << '\n';
    }

    REQUIRE(csv.good());
}
