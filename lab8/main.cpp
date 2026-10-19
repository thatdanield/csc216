#include "JobQueue.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>

TEST_CASE("Jobs compare by priority", "[job]") {
    Job lowPriority("low", 1);
    Job highPriority("high", 10);

    REQUIRE(lowPriority < highPriority);
    REQUIRE_FALSE(highPriority < lowPriority);
}

TEST_CASE("JobQueue runs jobs from highest to lowest priority", "[job-queue]") {
    Job lowPriority("low", 1);
    Job highPriority("high", 10);
    Job mediumPriority("medium", 5);
    JobQueue queue;
    queue.insert(&lowPriority);
    queue.insert(&highPriority);
    queue.insert(&mediumPriority);

    std::ostringstream output;
    std::streambuf* previousOutput = std::cout.rdbuf(output.rdbuf());
    queue.runHighestPriority();
    queue.runHighestPriority();
    queue.runHighestPriority();
    std::cout.rdbuf(previousOutput);

    const std::string result = output.str();
    const auto highPosition = result.find("name high");
    const auto mediumPosition = result.find("name medium");
    const auto lowPosition = result.find("name low");

    REQUIRE(highPosition != std::string::npos);
    REQUIRE(mediumPosition != std::string::npos);
    REQUIRE(lowPosition != std::string::npos);
    REQUIRE(highPosition < mediumPosition);
    REQUIRE(mediumPosition < lowPosition);
}
