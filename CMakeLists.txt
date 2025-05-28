cmake_minimum_required(VERSION 3.15)
project(WorseAlternativeTradingSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

file(GLOB_RECURSE MAIN_SOURCES
    "${CMAKE_SOURCE_DIR}/matchingEngine/*.cpp"
    "${CMAKE_SOURCE_DIR}/order/*.cpp"
    "${CMAKE_SOURCE_DIR}/pitchMessage/*.cpp"
    "${CMAKE_SOURCE_DIR}/traderPool/*.cpp"
    "${CMAKE_SOURCE_DIR}/dataService/*.cpp"
    "${CMAKE_SOURCE_DIR}/sim/*.cpp"
)

add_executable(WorseAlternativeTradingSystem ${MAIN_SOURCES})

target_include_directories(WorseAlternativeTradingSystem PRIVATE
    "${CMAKE_SOURCE_DIR}/matchingEngine"
    "${CMAKE_SOURCE_DIR}/order"
    "${CMAKE_SOURCE_DIR}/pitchMessage"
    "${CMAKE_SOURCE_DIR}/traderPool"
    "${CMAKE_SOURCE_DIR}/dataService"
    "${CMAKE_SOURCE_DIR}/sim"
    "C:/local/boost_1_88_0"
)

# TESTING
enable_testing()

set(BOOST_ROOT "C:/local/boost_1_88_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}")

file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/matchingEngine/*.cpp"
    "${CMAKE_SOURCE_DIR}/order/*.cpp"
    "${CMAKE_SOURCE_DIR}/pitchMessage/*.cpp"
    "${CMAKE_SOURCE_DIR}/traderPool/*.cpp"
    "${CMAKE_SOURCE_DIR}/dataService/*.cpp"
    "${CMAKE_SOURCE_DIR}/test/*.cpp"
)

add_executable(UnitTests ${TEST_SOURCES})

target_include_directories(UnitTests PRIVATE
    "${CMAKE_SOURCE_DIR}/matchingEngine"
    "${CMAKE_SOURCE_DIR}/order"
    "${CMAKE_SOURCE_DIR}/pitchMessage"
    "${CMAKE_SOURCE_DIR}/traderPool"
    "${CMAKE_SOURCE_DIR}/dataService"
    "${CMAKE_SOURCE_DIR}/sim"
    "${CMAKE_SOURCE_DIR}/test"
    "C:/local/boost_1_88_0"
)

add_test(NAME AllUnitTests COMMAND UnitTests)