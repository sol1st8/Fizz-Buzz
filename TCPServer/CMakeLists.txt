cmake_minimum_required(VERSION 3.11)

project(server CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(server main.cpp)

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
  set(SYSTEM_LIBS -lstdc++)
else()
  set(SYSTEM_LIBS)
endif()

target_include_directories(server PRIVATE CONAN_PKG::boost)
target_link_libraries(server PRIVATE Threads::Threads CONAN_PKG::boost)
target_link_libraries(server ${SYSTEM_LIBS})
