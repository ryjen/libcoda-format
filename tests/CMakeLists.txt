
add_executable(${TEST_PROJECT_NAME} main.test.cpp format.test.cpp)

target_include_directories(${TEST_PROJECT_NAME} SYSTEM PUBLIC vendor/bandit SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries (${TEST_PROJECT_NAME} ${PROJECT_NAME})


