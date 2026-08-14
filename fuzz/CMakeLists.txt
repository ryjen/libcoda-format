add_executable(coda_format_afl format_afl.cpp)

target_link_libraries(coda_format_afl PRIVATE ${PROJECT_NAME})
target_compile_features(coda_format_afl PRIVATE cxx_std_17)
