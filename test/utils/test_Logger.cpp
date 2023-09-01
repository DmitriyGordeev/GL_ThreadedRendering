#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Logger.h"


class TestLogger: public Logger {
public:
    static std::shared_ptr<Logger> test_getInstance() {
        return m_Instance;
    }
};

std::shared_ptr<Logger> Logger::m_Instance = nullptr;


TEST_CASE( "Logger_1", "[getLogger()]" ) {
    REQUIRE(TestLogger::test_getInstance() == nullptr);
    auto wp = TestLogger::getLogger();
    REQUIRE(TestLogger::test_getInstance() != nullptr);
}

TEST_CASE( "Logger_2", "[info]" ) {
    REQUIRE(TestLogger::test_getInstance() == nullptr);
    TestLogger::info("info");
    REQUIRE(TestLogger::test_getInstance() != nullptr);
}

TEST_CASE( "Logger_3", "[error]" ) {
    REQUIRE(TestLogger::test_getInstance() == nullptr);
    TestLogger::error("error");
    REQUIRE(TestLogger::test_getInstance() != nullptr);
}