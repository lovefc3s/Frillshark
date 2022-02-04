/*
	This file FrillShark Odbc C++ source Generation.
	"testdb"".hpp"
	Set_Driver("ODBC Driver 17 for SQL Server");
	Set_Server("192.168.0.???");
	Set_UserID("testuser");
	Set_Password(" .... ");
	Set_Database("testdb");
	Microsoft SQL Server 2019 (RTM-CU13) (KB5005679) - 15.0.4178.1 (X64)
	Sep 23 2021 16:47:49
	Copyright (C) 2019 Microsoft Corporation
	Express Edition (64-bit) on Linux (Ubuntu 21.10) <X64>
*/

#ifndef __TESTDB_HPP__
#define __TESTDB_HPP__
#include "odbccommon_testdb.hpp"

using namespace OdbcCommon;
class CR_t_testtable : public COdbcRecord {
public:
	CR_t_testtable() : COdbcRecord() { Initialize(); }
	virtual ~CR_t_testtable() {}

	void Initialize() {
		id = 0;
		name = "";
	}

public:
	SQLINTEGER id;
	std::string name;
};
class CT_t_testtable : public COdbcTable {
public:
	CT_t_testtable() : COdbcTable() {
		m_SqlSELECT = "SELECT "
					  "id,"
					  "name"
					  " FROM t_testtable";
		m_SqlINSERT = "INSERT INTO t_testtable ("
					  "id,"
					  "name) VALUES "
					  "( ?,? )";
		m_SqlUPDATE = "UPDATE t_testtable SET "
					  "id = ?,"
					  "name = ?";

		m_SqlDELETE = "DELETE t_testtable ";
		COdbcColumn col;
		col.SetValue("testdb", "dbo", "t_testtable", "id", "1", "", "NO", "int",
					 "0", "0", "10", "0", "0", "", "", _int);
		m_Column.push_back(col);
		col.SetValue("testdb", "dbo", "t_testtable", "name", "2", "", "YES",
					 "varchar", "50", "50", "0", "0", "0", "cp932",
					 "Japanese_CI_AS", _varchar);
		m_Column.push_back(col);
	}
	virtual ~CT_t_testtable() { m_Data.clear(); }

	CR_t_testtable operator[](int n) { return m_Data.at(n); }

public:
	std::vector<CR_t_testtable> m_Data;
};
#endif
