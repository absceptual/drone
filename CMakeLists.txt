cmake_minimum_required(VERSION 3.0.0)
project(Drone VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB SOURCES
    client/headers/*.h
    client/*.cpp
)
add_executable(Drone ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(Drone PRIVATE shared)
if(MINGW)
  target_link_libraries(Drone ws2_32)
endif()

