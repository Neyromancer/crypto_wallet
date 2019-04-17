cmake_minimum_required(VERSION 3.7...3.10)

if (${CMAKE_VERSION} VERSION_GREATER 3.10 OR ${CMAKE_VERSION} VERSION_LESS 3.7)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(client)

set(CMAKE_CXX_STANDARD 14)
set (project_src
     data_loader/data_loader.cpp
    )

find_package (sqlite3)
include_directories (${SQLITE3_INCLUDE_DIRS})
#include_directories (/usr/include)
#link_directories (/usr/bin)

add_executable (crypto_wallet_client ${project_src} ${SQLITE3_LIBRARIES})
#target_include_directories(crypto_wallet_test PRIVATE ${UTILS_INCLUDE_DIR})
#target_link_libraries(crypto_wallet_test)

#add_subdirectory(data_loader)

#add_executable (client
