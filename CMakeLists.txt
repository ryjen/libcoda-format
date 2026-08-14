cmake_minimum_required(VERSION 3.21)

project(coda_format VERSION 0.1.1 LANGUAGES CXX)

option(CODA_BUILD_TESTS "Build libcoda-format tests." ON)
option(CODA_BUILD_FUZZERS "Build deterministic fuzzing targets." OFF)
option(CODA_ENABLE_COVERAGE "Enable code coverage testing." OFF)
option(CODA_ENABLE_MEMCHECK "Enable Valgrind memory checking." OFF)
option(CODA_ENABLE_PROFILING "Enable Valgrind profiling." OFF)

# Backward-compatible aliases for existing scripts and aggregate builds.
option(ENABLE_COVERAGE "Deprecated: use CODA_ENABLE_COVERAGE." OFF)
option(ENABLE_MEMCHECK "Deprecated: use CODA_ENABLE_MEMCHECK." OFF)
option(ENABLE_PROFILING "Deprecated: use CODA_ENABLE_PROFILING." OFF)
mark_as_advanced(ENABLE_COVERAGE ENABLE_MEMCHECK ENABLE_PROFILING)

if(ENABLE_COVERAGE)
    set(CODA_ENABLE_COVERAGE ON)
endif()
if(ENABLE_MEMCHECK)
    set(CODA_ENABLE_MEMCHECK ON)
endif()
if(ENABLE_PROFILING)
    set(CODA_ENABLE_PROFILING ON)
endif()

set(ENABLE_COVERAGE "${CODA_ENABLE_COVERAGE}")
set(ENABLE_MEMCHECK "${CODA_ENABLE_MEMCHECK}")
set(ENABLE_PROFILING "${CODA_ENABLE_PROFILING}")

if(NOT CODA_BUILD_TESTS AND
   (CODA_ENABLE_COVERAGE OR CODA_ENABLE_MEMCHECK OR CODA_ENABLE_PROFILING))
    message(FATAL_ERROR
        "Coverage, memcheck, and profiling require CODA_BUILD_TESTS=ON"
    )
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

include(CreatePackageConfig)
create_package_config(DESCRIPTION "a string formatting library")

add_subdirectory(src)

if(CODA_BUILD_FUZZERS)
    add_subdirectory(fuzz)
endif()

if(CODA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)

    if(CODA_ENABLE_COVERAGE)
        include(CodaCoverage)
        coda_enable_target_coverage(TARGET ${PROJECT_NAME})
        coda_enable_target_coverage(TARGET ${TEST_PROJECT_NAME})
        setup_target_for_coverage(
            TARGET ${PROJECT_NAME}_coverage
            OUTPUT ${PROJECT_BINARY_DIR}/coverage
        )
    endif()

    include(ValgrindTest)
    add_valgrind_profile_test(
        MEMCHECK ${CODA_ENABLE_MEMCHECK}
        PROFILING ${CODA_ENABLE_PROFILING}
        TARGET ${TEST_PROJECT_NAME}
    )
endif()
