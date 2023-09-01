#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Logger.h"


std::shared_ptr<Logger> Logger::m_Instance = nullptr;

TEST_CASE( "Logger", "[getLogger()]" ) {
    REQUIRE(Logger::m_Instance == nullptr);
    auto wp = Logger::getLogger();
    REQUIRE(Logger::m_Instance != nullptr);
}
