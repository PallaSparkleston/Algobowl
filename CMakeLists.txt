cmake_minimum_required(VERSION 3.0.0)
project(ALGOBOWL)

set(SOURCES src/main.cpp src/Algorithm.cpp src/Validator.cpp src/Input.cpp)
set(H_FILES include/Algorithm.h include/Validator.h include/Input.h)

include_directories(include/)

add_executable(AlgoBowl
    ${SOURCES}
    ${H_FILES}
)


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/output)
file(COPY ${PROJECT_SOURCE_DIR}/input_data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/input_algorithm_output DESTINATION ${CMAKE_BINARY_DIR})

