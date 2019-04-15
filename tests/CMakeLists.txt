cmake_minimum_required(VERSION 3.7...3.10)

if (${CMAKE_VERSION} VERSION_GREATER 3.10 OR ${CMAKE_VERSION} VERSION_LESS 3.7)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set (CMAKE_CXX_STANDARD 14)
set (unittest_sources
     data_loader/test_data_loader.cpp
    )

find_package (Threads REQUIRED)
find_package (GTest REQUIRED)

include_directories (${GTEST_INCLUDE_DIRS})
include_directories (${CMAKE_WORKING_DIR})
include_directories (src/client)

# target_include_directories (data_loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (crypto_wallet_test ${unittest_sources})
target_include_directories(crypto_wallet_test PRIVATE ${UTILS_INCLUDE_DIR})
target_link_libraries (crypto_wallet_test ${GTEST_LIBRARIES} pthread)
