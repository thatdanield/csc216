#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <iterator>
#include <string>

bool canCreateNote(const std::string& note, const std::string articles[], int articleCount);

TEST_CASE("note can be created from characters across multiple articles") {
    const std::string articles[]{"I wrote a lot", "I read a lot."};

    REQUIRE(canCreateNote("tale", articles, 2));
}

TEST_CASE("note cannot use a character more times than articles provide") {
    const std::string articles[]{"one"};

    REQUIRE_FALSE(canCreateNote("eeee", articles, 1));
}

TEST_CASE("note creation counts punctuation and numbers") {
    const std::string articles[]{"Room 42: ready!"};

    REQUIRE_FALSE(canCreateNote("42!?", articles, 1));
    REQUIRE(canCreateNote("42!", articles, 1));
}

TEST_CASE("note matching is case insensitive") {
    const std::string uppercaseArticles[]{"HELLO"};
    const std::string lowercaseArticles[]{"hello"};

    REQUIRE(canCreateNote("hello", uppercaseArticles, 1));
    REQUIRE(canCreateNote("HELLO", lowercaseArticles, 1));
    REQUIRE(canCreateNote("HeLLo", lowercaseArticles, 1));
}

TEST_CASE("spaces do not need to be recreated") {
    const std::string articles[]{"ab"};

    REQUIRE(canCreateNote("a b", articles, 1));
}

TEST_CASE("empty note can always be created") {
    const std::string articles[]{"anything"};

    REQUIRE(canCreateNote("", articles, 1));
}

TEST_CASE("Othello cannot supply the full Romeo and Juliet text") {
    std::ifstream romeoFile("RomeoAndJuliet.txt");
    std::ifstream othelloFile("Othello.txt");

    REQUIRE(romeoFile.is_open());
    REQUIRE(othelloFile.is_open());

    const std::string romeoText(
        (std::istreambuf_iterator<char>(romeoFile)),
        std::istreambuf_iterator<char>());
    const std::string othelloText(
        (std::istreambuf_iterator<char>(othelloFile)),
        std::istreambuf_iterator<char>());
    const std::string articles[]{othelloText};

    REQUIRE_FALSE(canCreateNote(romeoText, articles, 1));
}
