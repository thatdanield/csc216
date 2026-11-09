#include "AVL.h"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <random>
#include <set>
#include <sstream>
#include <vector>

int checkSubtree(std::vector<int>& tree, int index, int minimum, int maximum,
                 std::vector<int>& orderedValues) {
    if (index >= tree.size() || tree[index] == -1)
        return 0;

    REQUIRE(tree[index] > minimum);
    REQUIRE(tree[index] < maximum);

    int leftHeight = checkSubtree(tree, 2 * index + 1, minimum, tree[index], orderedValues);
    orderedValues.push_back(tree[index]);
    int rightHeight = checkSubtree(tree, 2 * index + 2, tree[index], maximum, orderedValues);

    REQUIRE(leftHeight - rightHeight >= -1);
    REQUIRE(leftHeight - rightHeight <= 1);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

std::vector<int> checkTree(AVL& tree) {
    std::vector<int> values;
    std::stringstream serialized(tree.serialize());
    std::string value;

    while (std::getline(serialized, value, ','))
        values.push_back(std::stoi(value));

    std::vector<int> orderedValues;
    checkSubtree(values, 0, -1000000000, 1000000000, orderedValues);

    for (int index = 1; index < values.size(); index++)
        if (values[index] != -1)
            REQUIRE(values[(index - 1) / 2] != -1);

    return orderedValues;
}

TEST_CASE("new AVL trees serialize as empty") {
    AVL tree;

    REQUIRE(tree.serialize().empty());
}

TEST_CASE("insertion balances all four rotation cases") {
    SECTION("LL") {
        AVL tree;
        tree.insert(30);
        tree.insert(20);
        tree.insert(10);

        REQUIRE(tree.serialize() == "20,10,30");
    }

    SECTION("RR") {
        AVL tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);

        REQUIRE(tree.serialize() == "20,10,30");
    }

    SECTION("LR") {
        AVL tree;
        tree.insert(30);
        tree.insert(10);
        tree.insert(20);

        REQUIRE(tree.serialize() == "20,10,30");
    }

    SECTION("RL") {
        AVL tree;
        tree.insert(10);
        tree.insert(30);
        tree.insert(20);

        REQUIRE(tree.serialize() == "20,10,30");
    }
}

TEST_CASE("insertion ignores duplicate values") {
    AVL tree;
    tree.insert(5);
    tree.insert(5);

    REQUIRE(tree.serialize() == "5");
}

TEST_CASE("deletion removes a leaf") {
    AVL tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.del(10);

    REQUIRE(tree.serialize() == "20,-1,30");
}

TEST_CASE("deletion removes a node with two children") {
    AVL tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(25);
    tree.insert(35);
    tree.del(30);

    REQUIRE(tree.serialize() == "20,10,35,-1,-1,25");
    REQUIRE(checkTree(tree) == std::vector<int>{10, 20, 25, 35});
}

TEST_CASE("deleting a missing value leaves the tree unchanged") {
    AVL tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    std::string before = tree.serialize();

    tree.del(99);

    REQUIRE(tree.serialize() == before);
}

TEST_CASE("serialization round trips through deserialization") {
    AVL original;
    original.insert(40);
    original.insert(20);
    original.insert(60);
    original.insert(10);
    original.insert(30);
    original.insert(50);
    original.insert(70);
    std::string serialized = original.serialize();
    AVL restored;

    restored.deserialize(serialized);

    REQUIRE(restored.serialize() == serialized);
}

TEST_CASE("deserializing an empty string produces an empty tree") {
    AVL tree;
    tree.insert(1);

    tree.deserialize("");

    REQUIRE(tree.serialize().empty());
}

TEST_CASE("large ascending and descending insertions remain balanced") {
    for (int count : {127, 511, 1023, 2047}) {
        SECTION("ascending " + std::to_string(count)) {
            AVL tree;
            for (int value = 0; value < count; value++)
                tree.insert(value);

            std::vector<int> orderedValues = checkTree(tree);
            REQUIRE(orderedValues.size() == count);
            for (int value = 0; value < count; value++)
                REQUIRE(orderedValues[value] == value);
        }

        SECTION("descending " + std::to_string(count)) {
            AVL tree;
            for (int value = count - 1; value >= 0; value--)
                tree.insert(value);

            std::vector<int> orderedValues = checkTree(tree);
            REQUIRE(orderedValues.size() == count);
            for (int value = 0; value < count; value++)
                REQUIRE(orderedValues[value] == value);
        }
    }
}

TEST_CASE("large randomized insertion and deletion preserve AVL invariants") {
    std::mt19937 generator(216);
    for (int trial = 0; trial < 4; trial++) {
        int count = 500 + trial * 500;
        std::vector<int> values;
        for (int value = 0; value < count; value++)
            values.push_back(value);
        std::shuffle(values.begin(), values.end(), generator);

        AVL tree;
        std::set<int> expectedValues;
        for (int value : values) {
            tree.insert(value);
            expectedValues.insert(value);
            if (expectedValues.size() % 100 == 0)
                REQUIRE(checkTree(tree) == std::vector<int>(expectedValues.begin(), expectedValues.end()));
        }
        REQUIRE(checkTree(tree) == std::vector<int>(expectedValues.begin(), expectedValues.end()));

        std::shuffle(values.begin(), values.end(), generator);
        for (int value : values) {
            tree.del(value);
            expectedValues.erase(value);
            if (expectedValues.size() % 100 == 0)
                REQUIRE(checkTree(tree) == std::vector<int>(expectedValues.begin(), expectedValues.end()));
        }
        REQUIRE(tree.serialize().empty());
    }
}

TEST_CASE("mixed operations and repeated serialization preserve tree contents") {
    std::mt19937 generator(911);
    std::uniform_int_distribution<int> values(-500, 500);
    std::uniform_int_distribution<int> operations(0, 1);
    AVL tree;
    std::set<int> expectedValues;

    for (int operation = 0; operation < 3000; operation++) {
        int value = values(generator);
        if (value == -1)
            value = -2;
        if (operations(generator) == 0) {
            tree.insert(value);
            expectedValues.insert(value);
        } else {
            tree.del(value);
            expectedValues.erase(value);
        }

        if (operation % 50 == 0) {
            std::vector<int> orderedValues = checkTree(tree);
            REQUIRE(orderedValues == std::vector<int>(expectedValues.begin(), expectedValues.end()));

            AVL restored;
            restored.deserialize(tree.serialize());
            REQUIRE(restored.serialize() == tree.serialize());
            REQUIRE(checkTree(restored) == orderedValues);
        }
    }

    REQUIRE(checkTree(tree) == std::vector<int>(expectedValues.begin(), expectedValues.end()));
}
