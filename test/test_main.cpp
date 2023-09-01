#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iostream>
using std::cout;


TEST_CASE( "Experiment", "[func()]" ) {
    REQUIRE(4 == 4);
}