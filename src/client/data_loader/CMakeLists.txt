cmake_minimum_required(VERSION 3.7...3.10)

if (${CMAKE_VERSION} VERSION_GREATER 3.10 OR ${CMAKE_VERSION} VERSION_LESS 3.7)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(client_data_loader)

set(CMAKE_CXX_STANDARD 14)
set (project_src data_loader.cpp)

find_package (Threads REQUIRED)
include_directories (${SQLITE3_INCLUDE_DIRS})
## TODO avoid hard coding paths
set(SQLITE3_LIBRARIES /usr/lib/x86_64-linux-gnu/libsqlite3.a)

add_library (client_data_loader ${project_src})
target_link_libraries(client_data_loader ${SQLITE3_LIBRARIES} ${CMAKE_DL_LIBS} pthread)
