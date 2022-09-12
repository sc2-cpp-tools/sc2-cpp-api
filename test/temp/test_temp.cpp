#include "temp/temp.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Always returns 1", "[temp]") {
    REQUIRE(temp() == 1);
    REQUIRE(temp() != 2);
}
