
add_executable(${PROJECT_NAME}_test main.test.cpp format.test.cpp)

include_directories(SYSTEM vendor/bandit ${PROJECT_SOURCE_DIR}/src)

target_link_libraries (${PROJECT_NAME}_test ${PROJECT_NAME})


