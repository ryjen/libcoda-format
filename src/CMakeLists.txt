add_library(${PROJECT_NAME} format.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

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
