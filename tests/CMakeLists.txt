
add_executable(arg3format_test arg3.test.cpp format.test.cpp)

include_directories(SYSTEM vendor/bandit ../src)

target_link_libraries (arg3format_test arg3format)


