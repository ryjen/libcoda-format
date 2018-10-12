cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

# add options for testing
option(ENABLE_COVERAGE "Enable code coverage testing." OFF)
option(ENABLE_MEMCHECK "Enable testing for memory leaks." OFF)
option(ENABLE_PROFILING "Enable profiling code usage." OFF)

# define project name
project (coda_format VERSION 0.1.1)

# set path to custom modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# create the package config install
include(CreatePackage)
create_package(DESCRIPTION "a string formatting library")

# Setup testing
enable_testing()

# add directories
add_subdirectory(src)
add_subdirectory(tests)

###########################################################
# Create DEB
############################################################

# Tell CPack to generate a .deb package
set(CPACK_GENERATOR "DEB")

# Set a Package Maintainer.
# This is required
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ryan Jennings <ryan@micrantha.com>")

set(CPACK_PACKAGE_VENDOR "Micrantha Software Solutions")

# Set a Package Version
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A c++ toolkit")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# Include CPack
include(CPack)
