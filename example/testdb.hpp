/*
	This file FrillShark Odbc C++ source Generation.
	"testdb"".hpp"
	Set_Driver("ODBC Driver 17 for SQL Server");
	Set_Server("192.168.1.16");
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
		m_TableName = "t_testtable";
		m_SqlSELECT = "SELECT "
					  "id,"
					  "name"
					  " FROM t_testtable";
		m_SqlINSERT = "INSERT INTO t_testtable ("
					  "id,"
					  "name)"
					  " VALUES ( ?,?)";
		m_SqlUPDATE = "UPDATE t_testtable SET "
					  "name = ?";

		m_SqlDELETE = "DELETE t_testtable ";
		COdbcColumn col;
		col.SetValue("testdb", "dbo", "t_testtable", "id", "1", "", "NO", "int",
					 "0", "0", "10", "0", "0", "", "", 0, _int, 0);
		m_Column.push_back(col);
		col.SetValue("testdb", "dbo", "t_testtable", "name", "2", "", "YES",
					 "varchar", "50", "50", "0", "0", "0", "cp932",
					 "Japanese_CI_AS", 0, _varchar, -1);
		m_Column.push_back(col);
		m_Key.clear();
		COdbcKeyColumn key;
		key.Set_Value("PK__t_testta__3213E83FACC63898", "id", 1);
		m_Key.push_back(key);
	}
	virtual ~CT_t_testtable() { m_Data.clear(); }

public:
	SQLLEN Set_TableData(COdbcCommand *com) {
		SQLRETURN ret = SQL_SUCCESS;
		SQLLEN Count = 0;
		this->m_Data.clear();
		ret = com->mSQLExecDirect();
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) return -1;
		char *name = new char[51];
		for (int i = 0;; i++) {
			ret = com->mFetch();
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				CR_t_testtable rec;
				com->GetData(1, SQL_C_LONG, &rec.id, sizeof(rec.id), 0);
				memset(name, 0, 51);
				com->GetData(2, SQL_C_CHAR, name, 51, 0);
				rec.name = (char *)name;
				m_Data.push_back(rec);
				Count++;
			} else
				break;
		}
		delete[] name;
		return Count;
	}
	SQLLEN Set_TableData(COdbcCommand *com, std::string ConditionalFormula,
						 std::string OrderBy = "") {
		std::string sql = this->Get_SELECT();
		if (ConditionalFormula.length() > 0) {
			sql = sql + " WHERE " + ConditionalFormula;
		}
		if (OrderBy.length() > 0) {
			sql = sql + " ORDER BY " + OrderBy;
		}
		com->SetCommandString(sql);
		return this->Set_TableData(com);
	}
	CR_t_testtable operator[](int n) { return m_Data.at(n); }
	std::string WherePrimaryKey(CR_t_testtable &rec) {
		std::string sql = "";
		for (int j = 0; j < this->KeyCount(); j++) {
			if (j == 0)
				sql = " WHERE ";
			else
				sql = sql + " AND ";
			int pos = this->Key(j).KEY_ORDINAL_POSITION - 1;
			sql = sql + this->Key(j).KEY_COLUMN_NAME + " = '" + rec[pos] + "'";
		}
		return sql;
	}

public:
	std::vector<CR_t_testtable> m_Data;
};
class CR_t_named : public COdbcRecord {
public:
	CR_t_named() : COdbcRecord() { Initialize(); }
	virtual ~CR_t_named() {}

	void Initialize() {
		memset(autoid, 0, 48);
		name_id = "";
		name_value = "";
	}

public:
	SQLCHAR autoid[48];
	std::string name_id;
	std::string name_value;
};
class CT_t_named : public COdbcTable {
public:
	CT_t_named() : COdbcTable() {
		m_TableName = "t_named";
		m_SqlSELECT = "SELECT "
					  "autoid,"
					  "name_id,"
					  "name_value"
					  " FROM t_named";
		m_SqlINSERT = "INSERT INTO t_named ("
					  "name_id,"
					  "name_value)"
					  " VALUES ( ?,?)";
		m_SqlUPDATE = "UPDATE t_named SET "
					  "name_id = ?,"
					  "name_value = ?";

		m_SqlDELETE = "DELETE t_named ";
		COdbcColumn col;
		col.SetValue("testdb", "dbo", "t_named", "autoid", "1", "", "NO",
					 "numeric", "0", "0", "18", "0", "0", "", "", 1, _numeric,
					 0);
		m_Column.push_back(col);
		col.SetValue("testdb", "dbo", "t_named", "name_id", "2", "", "NO",
					 "nvarchar", "50", "100", "0", "0", "0", "UNICODE",
					 "Japanese_CI_AS", 0, _nvarchar, -1);
		m_Column.push_back(col);
		col.SetValue("testdb", "dbo", "t_named", "name_value", "3", "", "YES",
					 "nvarchar", "-1", "-1", "0", "0", "0", "UNICODE",
					 "Japanese_CI_AS", 0, _nvarchar, -1);
		m_Column.push_back(col);
		m_Key.clear();
		COdbcKeyColumn key;
		key.Set_Value("PK_t_nameed", "autoid", 1);
		m_Key.push_back(key);
	}
	virtual ~CT_t_named() { m_Data.clear(); }

public:
	SQLLEN Set_TableData(COdbcCommand *com) {
		SQLRETURN ret = SQL_SUCCESS;
		SQLLEN Count = 0;
		this->m_Data.clear();
		ret = com->mSQLExecDirect();
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) return -1;
		char *name_id = new char[101];
		char *name_value = new char[0];
		for (int i = 0;; i++) {
			ret = com->mFetch();
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				CR_t_named rec;
				com->GetData(1, SQL_C_CHAR, &rec.autoid, sizeof(rec.autoid), 0);
				memset(name_id, 0, 101);
				com->GetData(2, SQL_C_CHAR, name_id, 101, 0);
				rec.name_id = (char *)name_id;
				memset(name_value, 0, 0);
				com->GetData(3, SQL_C_CHAR, name_value, 0, 0);
				rec.name_value = (char *)name_value;
				m_Data.push_back(rec);
				Count++;
			} else
				break;
		}
		delete[] name_id;
		delete[] name_value;
		return Count;
	}
	SQLLEN Set_TableData(COdbcCommand *com, std::string ConditionalFormula,
						 std::string OrderBy = "") {
		std::string sql = this->Get_SELECT();
		if (ConditionalFormula.length() > 0) {
			sql = sql + " WHERE " + ConditionalFormula;
		}
		if (OrderBy.length() > 0) {
			sql = sql + " ORDER BY " + OrderBy;
		}
		com->SetCommandString(sql);
		return this->Set_TableData(com);
	}
	CR_t_named operator[](int n) { return m_Data.at(n); }
	std::string WherePrimaryKey(CR_t_named &rec) {
		std::string sql = "";
		for (int j = 0; j < this->KeyCount(); j++) {
			if (j == 0)
				sql = " WHERE ";
			else
				sql = sql + " AND ";
			int pos = this->Key(j).KEY_ORDINAL_POSITION - 1;
			sql = sql + this->Key(j).KEY_COLUMN_NAME + " = '" + rec[pos] + "'";
		}
		return sql;
	}

public:
	std::vector<CR_t_named> m_Data;
};
#endif
