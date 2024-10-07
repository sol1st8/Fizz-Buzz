cmake_minimum_required(VERSION 3.11)

project(client CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(client main.cpp)

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
  set(SYSTEM_LIBS -lstdc++)
else()
  set(SYSTEM_LIBS)
endif()

target_include_directories(client PRIVATE CONAN_PKG::boost)
target_link_libraries(client PRIVATE Threads::Threads CONAN_PKG::boost)
target_link_libraries(client ${SYSTEM_LIBS})
