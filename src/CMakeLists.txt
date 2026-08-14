add_library(${PROJECT_NAME} format.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/coda/format>
)

set(${PROJECT_NAME}_HEADERS
    format.h
)
string(REPLACE "_" "/" INSTALL_DIRECTORY ${PROJECT_NAME})

install(
    FILES ${${PROJECT_NAME}_HEADERS}
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${INSTALL_DIRECTORY}"
)

install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
