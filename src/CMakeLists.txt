cmake_minimum_required(VERSION 3.10)
project(Generic_A_Star VERSION 1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_library(astar INTERFACE)
target_include_directories(astar INTERFACE 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
add_library(astar::astar ALIAS astar)

# Install the target
install(TARGETS astar
        EXPORT astarTarget
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

#install the includes
install(FILES astar.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#install the cmake file for importing
install(EXPORT astarTarget
        FILE astarTarget.cmake
        NAMESPACE astar::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/astar
)

# Install the config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/astarConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/astar
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/astarConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/astar
)
