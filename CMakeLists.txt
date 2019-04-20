cmake_minimum_required (VERSION 3.7...3.10)

if (${CMAKE_VERSION} VERSION_GREATER 3.10 OR ${CMAKE_VERSION} VERSION_LESS 3.7)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(crypto_wallet)

set (CMAKE_CXX_STANDARD 14)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src/client)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_subdirectory (src/client)
add_subdirectory (tests)
