#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#define main lab10_main_for_test
#include "main.cpp"
#undef main

namespace {

class TemporaryInputFile {
public:
    explicit TemporaryInputFile(const std::string& contents) {
        static int nextId = 0;
        path = std::filesystem::temp_directory_path()
                / ("lab10-index-test-"
                   + std::to_string(std::chrono::steady_clock::now()
                                            .time_since_epoch()
                                            .count())
                   + "-" + std::to_string(nextId++) + ".txt");

        std::ofstream file(path);
        if (!file || !(file << contents)) {
            throw std::runtime_error("Could not create temporary test input");
        }
    }

    ~TemporaryInputFile() {
        std::error_code error;
        std::filesystem::remove(path, error);
        std::filesystem::remove(getOutputPath(), error);
    }

    const std::filesystem::path& getPath() const {
        return path;
    }

    std::filesystem::path getOutputPath() const {
        return path.string() + ".index";
    }

private:
    std::filesystem::path path;
};

std::string runIndexForFile(const std::filesystem::path& path) {
    std::istringstream input(path.string() + '\n');
    std::ostringstream output;
    std::streambuf* originalInput = std::cin.rdbuf(input.rdbuf());
    std::streambuf* originalOutput = std::cout.rdbuf(output.rdbuf());

    lab10_main_for_test();

    std::cin.clear();
    std::cin.rdbuf(originalInput);
    std::cout.rdbuf(originalOutput);
    return output.str();
}

std::string readFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

std::size_t countLines(const std::string& text) {
    return static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n'));
}

} // namespace

TEST_CASE("repeated words have all of their token positions") {
    TemporaryInputFile file("apple banana apple");

    REQUIRE(runIndexForFile(file.getPath())
            == "Input file name: \napple: [0, 2]\nbanana: [1]\n");
}

TEST_CASE("punctuation is removed from words before indexing") {
    TemporaryInputFile file("Red, Blue! red. well-known high-quality");

    REQUIRE(runIndexForFile(file.getPath())
            == "Input file name: \nblue: [1]\nhighquality: [4]\nred: [0, 2]\n"
               "wellknown: [3]\n");
}

TEST_CASE("punctuation-only tokens do not become words but keep their position") {
    TemporaryInputFile file("!!! apple ... apple");

    REQUIRE(runIndexForFile(file.getPath())
            == "Input file name: \napple: [1, 3]\n");
}

TEST_CASE("empty input produces no index entries") {
    TemporaryInputFile file("");

    REQUIRE(runIndexForFile(file.getPath()) == "Input file name: \n");
}

TEST_CASE("common words are filtered and capitalization is ignored") {
    TemporaryInputFile file("Apple a and THE they to for you your apple");

    REQUIRE(runIndexForFile(file.getPath())
            == "Input file name: \napple: [0, 9]\n");
}

TEST_CASE("index is saved to a file") {
    TemporaryInputFile file("banana apple banana");

    runIndexForFile(file.getPath());

    REQUIRE(readFile(file.getOutputPath())
            == "apple: [1]\nbanana: [0, 2]\n");
}

TEST_CASE("long public-domain essay is indexed without losing its contents") {
    // The American Scholar by Ralph Waldo Emerson, sourced from Project Gutenberg:
    // https://www.gutenberg.org/cache/epub/16643/pg16643.txt
    const std::string output = runIndexForFile(ESSAY_FILE);

    REQUIRE(output.find("american: [1") != std::string::npos);
    REQUIRE(output.find("scholar: [2") != std::string::npos);
    REQUIRE(output.find("\nthe: [") == std::string::npos);
    REQUIRE(countLines(output) > 500);
}
