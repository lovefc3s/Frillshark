cmake_minimum_required(VERSION 3.0.0)
project(Frillshark VERSION 0.1.0)

include(CTest)
enable_testing()
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules(ODBC odbc REQUIRED)

#set (CMAKE_CXX_FLAGS "-g -fsanitize=address")
add_executable(Frillshark
 stdafx.hpp
 resources/odbccommon.hpp
 CppGen.hpp
 MainWindow.hpp
 stdafx.cpp
 CppGen.cpp
 MainWindow.cpp
 Frillshark.cpp
)
target_precompile_headers( ${PROJECT_NAME}
 PUBLIC stdafx.hpp resources/odbccommon.hpp
)
include_directories(
 ./resources
 ${GTKMM_INCLUDE_DIRS}
 ${ODBC_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME}
 PRIVATE
 sqlite3
 ${GTKMM_LIBRARIES}
 ${ODBC_LIBRARIES}
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
