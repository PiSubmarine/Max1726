cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(PiSubmarine.Max1726.Test LANGUAGES CXX)

set(PiSubmarine.Max1726.Test.Sources 
	"PiSubmarine/Max1726/Max1726Test.cpp")

enable_testing()

# spdlog
find_package(spdlog)
find_package(gtest)

add_executable(PiSubmarine.Max1726.Test ${PiSubmarine.Max1726.Test.Sources})
target_compile_features(PiSubmarine.Max1726.Test PRIVATE cxx_std_23)
target_link_libraries(PiSubmarine.Max1726.Test PRIVATE PiSubmarine.Max1726)
target_link_libraries(PiSubmarine.Max1726.Test PRIVATE spdlog::spdlog)
target_link_libraries(PiSubmarine.Max1726.Test PRIVATE GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(PiSubmarine.Max1726.Test)