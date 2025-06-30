include("${_build_cmake_local}")

add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../public")

PiSubmarineAddDependency("https://github.com/PiSubmarine/RegUtils" "")
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE PiSubmarine.RegUtils)

PiSubmarineAddDependency("https://github.com/PiSubmarine/Api.Internal.I2C" "")
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE PiSubmarine.Api.Internal.I2C)