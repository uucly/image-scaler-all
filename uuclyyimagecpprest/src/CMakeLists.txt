set(BINARY ${CMAKE_PROJECT_NAME})

message("Soucre ${CMAKE_CURRENT_SOURCE_DIR}")
message("Soucre ${CMAKE_SOURCE_DIR}")

set(QUANT ${CMAKE_SOURCE_DIR}/libimagequant.a)
message("Projekt name ${CMAKE_PROJECT_NAME}")
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})
message("Sources ${SOURCES}")

add_executable(${BINARY}_run ${SOURCES})

#target_link_libraries(${BINARY}_run PUBLIC ${OpenCV_LIBS} cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
target_link_libraries(${BINARY}_run PUBLIC ${OpenCV_LIBS} cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal ${VIPS_CPP_LINK_LIBRARIES} ${QUANT})
# cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal
add_library(${BINARY}_lib STATIC ${SOURCES})
