#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "IO.h"


TEST_CASE( "IO class", "[readFile()]" ) {
    std::string content;
    bool result = IO::readFile("../../../test/utils/file.txt", content);
    REQUIRE(result == true);
    REQUIRE(content == "abc\n");
}
