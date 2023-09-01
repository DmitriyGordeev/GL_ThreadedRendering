#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <SDL.h>
#include "Scene.h"
#include <fstream>
#include "Logger.h"

std::shared_ptr<Logger> Logger::m_Instance = nullptr;

TEST_CASE( "Scene", "[add()]" ) {
    // TODO
}