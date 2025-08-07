cmake_minimum_required(VERSION 3.16)

project(PBF-FR-metrics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#########################################################

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/src/)

#########################################################

##Cinolib

if (USE_CINOLIB_GUI)
    add_definitions(-DUSE_CINOLIB_GUI)
    set(CINOLIB_USES_OPENGL_GLFW_IMGUI    ON)
endif()

set (cinolib_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/external/cinolib)
message (${cinolib_DIR})
find_package(cinolib REQUIRED)
link_libraries(cinolib)

include_directories(${cinolib_DIR}/include)
include_directories(${cinolib_DIR}/external/eigen)

#########################################################

## Deps
#include(${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/external/CMakeLists.txt)

#########################################################

## TCLAP
set (tclap_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/external/tclap)
include_directories(${tclap_DIR}/include)

#########################################################

##libLAS
set (libLAS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/vcpkg_cpptruths/installed/x64-windows/include)
set(libLAS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/vcpkg_cpptruths/installed/x64-windows/lib/liblas.lib)
include_directories(${libLAS_INCLUDE_DIR})

#########################################################

##GDAL
set (GDAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/external/gdal/install-Windows/include/)
set (GDAL_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../partitioning/external/gdal/install-Windows/lib/gdal.lib)
include_directories(${GDAL_INCLUDE_DIR})

########################################################

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GDAL_LIB} ${libLAS_LIBRARY})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
