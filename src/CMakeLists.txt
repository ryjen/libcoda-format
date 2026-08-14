add_library(${PROJECT_NAME} format.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/coda/format>
)

install(
    FILES "${PROJECT_SOURCE_DIR}/include/coda/format/format.h"
    DESTINATION include/coda/format
)

install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
