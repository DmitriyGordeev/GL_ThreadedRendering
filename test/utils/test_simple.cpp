#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Logger.h"
#include <vector>


std::shared_ptr<Logger> Logger::m_Instance = nullptr;


TEST_CASE( "Vector", "[constref]" ) {
}