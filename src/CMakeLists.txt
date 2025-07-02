include("${_build_cmake_local}")

add_library(PiSubmarine.Max1726 "PiSubmarine/Max1726/Max1726.cpp")
target_include_directories(PiSubmarine.Max1726 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../public")

PiSubmarineAddDependency("https://github.com/PiSubmarine/RegUtils" "")
target_link_libraries(PiSubmarine.Max1726 PUBLIC PiSubmarine.RegUtils)

PiSubmarineAddDependency("https://github.com/PiSubmarine/Api.Internal.I2C" "")
target_link_libraries(PiSubmarine.Max1726 PUBLIC PiSubmarine.Api.Internal.I2C)