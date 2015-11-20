cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

# set path to custom modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# add options for testing
option(CODE_COVERAGE "Enable code coverage testing." OFF)
option(MEMORY_CHECK "Enable testing for memory leaks." OFF)

if(CODE_COVERAGE)
	include(CodeCoverage)
	list(APPEND CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_COVERAGE})
endif()

# define project name
project (arg3format)

# add directories
add_subdirectory(src)
add_subdirectory(tests)

# Setup testing
enable_testing()

if (MEMORY_CHECK)
	include(MemCheckTest)
	add_memcheck_test(format-test ${CMAKE_BINARY_DIR}/tests/arg3format_test)
else ()
	add_test(format-test ${CMAKE_BINARY_DIR}/tests/arg3format_test)
endif()

# add target for code coverage
if(CODE_COVERAGE)
	setup_target_for_coverage(format-coverage ${CMAKE_BINARY_DIR}/tests/arg3format_test ${CMAKE_SOURCE_DIR}/coverage)
endif()

