#include <catch2/catch_test_macros.hpp>

#include "HashMap.h"

TEST_CASE("new maps have zero counts") {
    const HashMap map;

    REQUIRE(map.count('a') == 0);
    REQUIRE(map.count('7') == 0);
    REQUIRE(map.count('?') == 0);
}

TEST_CASE("adding characters tracks each character's count") {
    HashMap map;

    map.add('a');
    map.add('a');
    map.add('b');

    REQUIRE(map.count('a') == 2);
    REQUIRE(map.count('b') == 1);
}

TEST_CASE("adding text counts supported characters") {
    HashMap map;
    map.add("Hello, World! 42");

    REQUIRE(map.count('h') == 1);
    REQUIRE(map.count('l') == 3);
    REQUIRE(map.count('o') == 2);
    REQUIRE(map.count(',') == 1);
    REQUIRE(map.count('!') == 1);
    REQUIRE(map.count('4') == 1);
    REQUIRE(map.count('2') == 1);
    REQUIRE(map.count(' ') == 0);
}

TEST_CASE("uppercase letters are counted as lowercase") {
    HashMap map;
    map.add('A');
    map.add('a');

    REQUIRE(map.count('a') == 2);
    REQUIRE(map.count('A') == 2);
}

TEST_CASE("removing a character decrements its count") {
    HashMap map;
    map.add("apple");

    map.remove('p');

    REQUIRE(map.count('p') == 1);
    REQUIRE(map.count('a') == 1);
}

TEST_CASE("removing a missing character does not underflow") {
    HashMap map;

    map.remove('z');

    REQUIRE(map.count('z') == 0);
}

TEST_CASE("unsupported ASCII characters have no count") {
    HashMap map;
    map.add(' ');
    map.add('\n');
    map.add(static_cast<char>(0x7f));

    REQUIRE(map.count(' ') == 0);
    REQUIRE(map.count('\n') == 0);
    REQUIRE(map.count(static_cast<char>(0x7f)) == 0);
}
