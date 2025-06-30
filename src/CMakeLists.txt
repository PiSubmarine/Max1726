include("${_build_cmake_local}")

add_library(PiSubmarine.Max1726 INTERFACE)
target_include_directories(PiSubmarine.Max1726 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../public")

PiSubmarineAddDependency("https://github.com/PiSubmarine/RegUtils" "")
target_link_libraries(PiSubmarine.Max1726 INTERFACE PiSubmarine.RegUtils)

PiSubmarineAddDependency("https://github.com/PiSubmarine/Api.Internal.I2C" "")
target_link_libraries(PiSubmarine.Max1726 INTERFACE PiSubmarine.Api.Internal.I2C)