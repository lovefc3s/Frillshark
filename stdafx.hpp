#ifndef BOOST_EXCEPTION_DISABLE
#define BOOST_EXCEPTION_DISABLE
#endif
#ifndef BOOST_NO_EXCEPTIONS
#define BOOST_NO_EXCEPTIONS
#endif
#ifndef __STDAFX_HPP__
#define __STDAFX_HPP__
#ifndef __SQLITEFILE__
#define __SQLITEFILE__ "./resources/Frillshark.sqlite3"
#endif //__SQLITEFILE__
#include <glibmm.h>
#include <gtkmm.h>
#include <gtkmm/window.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include <sql.h>
#include <sqlext.h>
#include <sqlite3.h>

using namespace std;

#endif
