cmake_minimum_required(VERSION 3.0.0)
project(Howto VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ODBC odbc REQUIRED)
add_executable(${PROJECT_NAME}
 testdb.hpp
 
 Howtouseclass_main.cpp
)
target_precompile_headers( ${PROJECT_NAME}
 PUBLIC testdb.hpp
)
include_directories(
 ${ODBC_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME}
 ${ODBC_LIBRARIES}
)
