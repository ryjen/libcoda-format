
add_executable(${PROJECT_NAME}-test arg3.test.cpp format.test.cpp)

include_directories(SYSTEM vendor/bandit ${PROJECT_SOURCE_DIR}/src)

target_link_libraries (${PROJECT_NAME}-test ${PROJECT_NAME})


