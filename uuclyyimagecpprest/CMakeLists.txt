cmake_minimum_required (VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

message("current source root ${CMAKE_CURRENT_SOURCE_DIR}")
project ("mycmaketest")

message("opencv include ${OpenCV_INCLUDE_DIRS}")

find_package(PkgConfig)
#include(FindPkgConfig)
#pkg_check_modules( VIPS vips )
pkg_check_modules( VIPS_CPP vips-cpp )

#pkg_search_module( VIPS vips )
#pkg_search_module( VIPS_CPP vips-cpp )

message("Vips CPP Libraries ${VIPS_CPP_LIBRARIES}")
message("Vips Libraries ${VIPS_LIBRARIES}")
message("Vips CPP link libraries ${VIPS_CPP_LINK_LIBRARIES}")
message("Vips link libraries ${VIPS_LINK_LIBRARIES}")
message("Vips link libraries ${VIPS_LDFLAGS}")
message("VIPS CPP dir ${VIPS_CPP_INCLUDE_DIRS}")

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    message("-- OpecnCV found: ${OpenCV_VERSION_STRING}")
else()
    message(FATAL_ERROR "OpenCV not found!")
endif()

find_package(cpprestsdk REQUIRED)

include_directories(src ${OpenCV_INCLUDE_DIRS} ${VIPS_CPP_INCLUDE_DIRS})
add_subdirectory(src)
add_subdirectory(tests)

#set(BINARY_RUN ${CMAKE_PROJECT_NAME})

#link_libraries(PUBLIC ${OpenCV_LIBS} cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal ${${VIPS_CPP_LINK_LIBRARIES}})

