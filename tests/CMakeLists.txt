
if (NOT DEFINED BANDIT_DIR)

    include(ExternalProject)

    externalproject_add(${PROJECT_NAME}-bandit
            GIT_REPOSITORY https://github.com/banditcpp/bandit.git
            GIT_TAG v2.0.0
            INSTALL_COMMAND ""
            BUILD_COMMAND ""
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/bandit)

    ExternalProject_Get_property(${PROJECT_NAME}-bandit SOURCE_DIR)

    set(BANDIT_DIR ${SOURCE_DIR})

    message(STATUS "Using ${BANDIT_DIR}")
endif()

add_executable(${TEST_PROJECT_NAME} main.test.cpp format.test.cpp)

target_include_directories(${TEST_PROJECT_NAME} SYSTEM PUBLIC ${BANDIT_DIR} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries (${TEST_PROJECT_NAME} ${PROJECT_NAME})


