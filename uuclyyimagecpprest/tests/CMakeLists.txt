set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

enable_testing()
add_test(NAME ${BINARY} COMMAND ${BINARY})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib ${OpenCV_LIBS} ${VIPS_CPP_LINK_LIBRARIES} ${QUANT} GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
