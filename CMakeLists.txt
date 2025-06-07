cmake_minimum_required(VERSION 3.15)
project(WorseAlternativeTradingSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(BOOST_ROOT "C:/local/boost_1_88_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

file(GLOB_RECURSE MAIN_SOURCES
    "${CMAKE_SOURCE_DIR}/matchingEngine/*.cpp"
    "${CMAKE_SOURCE_DIR}/order/*.cpp"
    "${CMAKE_SOURCE_DIR}/pitchMessage/*.cpp"
    "${CMAKE_SOURCE_DIR}/traderPool/*.cpp"
    "${CMAKE_SOURCE_DIR}/dataService/*.cpp"
    "${CMAKE_SOURCE_DIR}/engineRouter/*.cpp"
    "${CMAKE_SOURCE_DIR}/sim/*.cpp"
)

add_executable(WorseAlternativeTradingSystem ${MAIN_SOURCES})
target_include_directories(WorseAlternativeTradingSystem PRIVATE
    "${CMAKE_SOURCE_DIR}/matchingEngine"
    "${CMAKE_SOURCE_DIR}/order"
    "${CMAKE_SOURCE_DIR}/pitchMessage"
    "${CMAKE_SOURCE_DIR}/traderPool"
    "${CMAKE_SOURCE_DIR}/dataService"
    "${CMAKE_SOURCE_DIR}/engineRouter"
    "${CMAKE_SOURCE_DIR}/sim"
    "${BOOST_INCLUDEDIR}"
)

# TESTING
enable_testing()

file(GLOB MATCHING_ENGINE_SOURCES "${CMAKE_SOURCE_DIR}/matchingEngine/*.cpp")
file(GLOB ORDER_SOURCES "${CMAKE_SOURCE_DIR}/order/*.cpp")
file(GLOB PITCH_MESSAGE_SOURCES "${CMAKE_SOURCE_DIR}/pitchMessage/*.cpp")

add_executable(matchingEngineTest 
    test/matchingEngineTest.cpp
    ${MATCHING_ENGINE_SOURCES}
    ${ORDER_SOURCES}
    ${PITCH_MESSAGE_SOURCES}
)
target_include_directories(matchingEngineTest PRIVATE
    "${CMAKE_SOURCE_DIR}/matchingEngine"
    "${CMAKE_SOURCE_DIR}/order"
    "${CMAKE_SOURCE_DIR}/pitchMessage"
    "${CMAKE_SOURCE_DIR}/test"
    "${BOOST_INCLUDEDIR}"
)
target_link_libraries(matchingEngineTest ${Boost_LIBRARIES})
add_test(NAME matchingEngineTest COMMAND matchingEngineTest)

add_executable(pitchMessageTest 
    test/pitchMessageTest.cpp
    ${PITCH_MESSAGE_SOURCES}
)
target_include_directories(pitchMessageTest PRIVATE
    "${CMAKE_SOURCE_DIR}/pitchMessage"
    "${CMAKE_SOURCE_DIR}/test"
    "${BOOST_INCLUDEDIR}"
)
target_link_libraries(pitchMessageTest ${Boost_LIBRARIES})
add_test(NAME pitchMessageTest COMMAND pitchMessageTest)