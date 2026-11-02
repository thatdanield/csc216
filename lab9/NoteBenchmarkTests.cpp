#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <string>

bool canCreateNote(const std::string& note, const std::string articles[], int articleCount);

TEST_CASE("benchmark note creation across logarithmic input sizes") {
    std::ifstream noteFile("RomeoAndJuliet.txt");
    REQUIRE(noteFile.is_open());

    const std::string fullNote(
        (std::istreambuf_iterator<char>(noteFile)),
        std::istreambuf_iterator<char>());

    std::ofstream resultsFile("CanCreateNoteBenchmark.csv");
    REQUIRE(resultsFile.is_open());
    resultsFile << "note_length,article_count,iterations,average_microseconds,result\n";
    resultsFile << std::fixed << std::setprecision(3);

    const int entryCountPerDimension = 10;
    const int articleArraySize = 100;
    const int iterations = 20;
    int entryCount = 0;

    for (int noteIndex = 0; noteIndex < entryCountPerDimension; noteIndex++) {
        double notePosition = static_cast<double>(noteIndex) /
                             static_cast<double>(entryCountPerDimension - 1);
        std::string::size_type noteLength = static_cast<std::string::size_type>(
            std::pow(static_cast<double>(fullNote.size()), notePosition) + 0.5);
        if (noteLength == 0) {
            noteLength = 1;
        }

        const std::string note = fullNote.substr(0, noteLength);

        for (int articleIndex = 0; articleIndex < entryCountPerDimension; articleIndex++) {
            double articlePosition = static_cast<double>(articleIndex) /
                                    static_cast<double>(entryCountPerDimension - 1);
            int articleCount = static_cast<int>(
                std::pow(static_cast<double>(articleArraySize), articlePosition) + 0.5);
            if (articleCount < 1) {
                articleCount = 1;
            }

            std::string articles[articleArraySize];
            for (int currentArticle = 0; currentArticle < articleCount; currentArticle++) {
                std::string::size_type start =
                    (note.size() * static_cast<std::string::size_type>(currentArticle)) /
                    static_cast<std::string::size_type>(articleCount);
                std::string::size_type end =
                    (note.size() * static_cast<std::string::size_type>(currentArticle + 1)) /
                    static_cast<std::string::size_type>(articleCount);
                articles[currentArticle] = note.substr(start, end - start);
            }

            const std::chrono::steady_clock::time_point startTime =
                std::chrono::steady_clock::now();
            bool result = false;
            for (int iteration = 0; iteration < iterations; iteration++) {
                result = canCreateNote(note, articles, articleCount);
            }
            const std::chrono::steady_clock::time_point endTime =
                std::chrono::steady_clock::now();
            const std::chrono::duration<double, std::micro> elapsed =
                endTime - startTime;

            REQUIRE(result);
            resultsFile << note.size() << ','
                        << articleCount << ','
                        << iterations << ','
                        << elapsed.count() / static_cast<double>(iterations) << ','
                        << (result ? "yes" : "no") << '\n';
            entryCount++;
        }
    }

    REQUIRE(entryCount == 100);
    REQUIRE(resultsFile.good());
}
