#ifndef __CPPGEN_HPP__
#define __CPPGEN_HPP__
#include "resources/odbccommon.hpp"
#include "stdafx.hpp"

#define Tab "\t"
#define NL "\n"
using namespace OdbcCommon;

class CCharBuffer {
public:
	CCharBuffer();
	virtual ~CCharBuffer();

public:
	int position;
	int length;
	std::string name;
};
class CR_INFORMATION_SCHEMA_COLUMNS : public COdbcRecord {
public:
	CR_INFORMATION_SCHEMA_COLUMNS() : COdbcRecord() { Initialize(); }
	virtual ~CR_INFORMATION_SCHEMA_COLUMNS() {}
	void Initialize() {
		memset(TABLE_CATALOG, 0, 256);
		memset(TABLE_SCHEMA, 0, 256);
		memset(TABLE_NAME, 0, 256);
		memset(COLUMN_NAME, 0, 256);
		ORDINAL_POSITION = 0;
		memset(COLUMN_DEFAULT, 0, 256);
		memset(IS_NULLABLE, 0, 8);
		memset(DATA_TYPE, 0, 256);
		CHARACTER_MAXIMUM_LENGTH = 0;
		CHARACTER_OCTET_LENGTH = 0;
		NUMERIC_PRECISION = 0;
		NUMERIC_SCALE = 0;
		DATETIME_PRECISION = 0;
		memset(CHARACTER_SET_NAME, 0, 256);
		memset(COLLATION_NAME, 0, 256);
		sqltype = eSqlType::_unknown;
		mIdentity = 0;
		mLength = 0;
	}

public:
	SQLCHAR TABLE_CATALOG[256];
	SQLCHAR TABLE_SCHEMA[256];
	SQLCHAR TABLE_NAME[256];
	SQLCHAR COLUMN_NAME[256];
	SQLINTEGER ORDINAL_POSITION;
	SQLCHAR COLUMN_DEFAULT[256];
	SQLCHAR IS_NULLABLE[8];
	SQLCHAR DATA_TYPE[256];
	SQLINTEGER CHARACTER_MAXIMUM_LENGTH;
	SQLINTEGER CHARACTER_OCTET_LENGTH;
	SQLINTEGER NUMERIC_PRECISION;
	SQLINTEGER NUMERIC_SCALE;
	SQLINTEGER DATETIME_PRECISION;
	SQLCHAR CHARACTER_SET_NAME[256];
	SQLCHAR COLLATION_NAME[256];
	OdbcCommon::eSqlType sqltype;
	int mIdentity;
	int mLength;

public:
	std::string operator[](int i) {
		std::string ret = "";
		std::stringstream ss;
		if (i < 0 || i > 14) return ret;
		switch (i) {
		case 0:
			ss << TABLE_CATALOG;
			break;
		case 1:
			ss << TABLE_SCHEMA;
			break;
		case 2:
			ss << TABLE_NAME;
			break;
		case 3:
			ss << COLUMN_NAME;
			break;
		case 4:
			ss << ORDINAL_POSITION;
			break;
		case 5:
			ss << COLUMN_DEFAULT;
			break;
		case 6:
			ss << IS_NULLABLE;
			break;
		case 7:
			ss << DATA_TYPE;
			break;
		case 8:
			ss << CHARACTER_MAXIMUM_LENGTH;
			break;
		case 9:
			ss << CHARACTER_OCTET_LENGTH;
			break;
		case 10:
			ss << NUMERIC_PRECISION;
			break;
		case 11:
			ss << NUMERIC_SCALE;
			break;
		case 12:
			ss << DATETIME_PRECISION;
			break;
		case 13:
			ss << CHARACTER_SET_NAME;
			break;
		case 14:
			ss << COLLATION_NAME;
			break;
		default:
			break;
		}
		ret = ss.str();
		return ret;
	}
};
class CT_INFORMATION_SCHEMA_COLUMNS : public COdbcTable {
public:
	CT_INFORMATION_SCHEMA_COLUMNS() : COdbcTable() {
		m_SqlSELECT = "SELECT TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME, "
					  "COLUMN_NAME, ORDINAL_POSITION, "
					  "COLUMN_DEFAULT, IS_NULLABLE, DATA_TYPE, "
					  "CHARACTER_MAXIMUM_LENGTH, CHARACTER_OCTET_LENGTH, "
					  "NUMERIC_PRECISION, NUMERIC_SCALE, DATETIME_PRECISION, "
					  "CHARACTER_SET_NAME, COLLATION_NAME "
					  "FROM INFORMATION_SCHEMA.COLUMNS ";
	}
	virtual ~CT_INFORMATION_SCHEMA_COLUMNS() {}
	SQLRETURN Set_Data(COdbcCommand *com, CR_INFORMATION_SCHEMA_COLUMNS *rec) {
		SQLRETURN ret;
		ret = com->GetData(1, SQL_C_CHAR, rec->TABLE_CATALOG, TABLE_CATALOGl, 0);
		ret = com->GetData(2, SQL_C_CHAR, rec->TABLE_SCHEMA, TABLE_SCHEMAl, 0);
		ret = com->GetData(3, SQL_C_CHAR, rec->TABLE_NAME, TABLE_NAMEl, 0);
		ret = com->GetData(4, SQL_C_CHAR, rec->COLUMN_NAME, COLUMN_NAMEl, 0);
		ret = com->GetData(5, SQL_C_LONG, &rec->ORDINAL_POSITION, ORDINAL_POSITIONl, 0);
		ret = com->GetData(6, SQL_C_CHAR, rec->COLUMN_DEFAULT, COLUMN_DEFAULTl, 0);
		ret = com->GetData(7, SQL_C_CHAR, rec->IS_NULLABLE, IS_NULLABLEl, 0);
		ret = com->GetData(8, SQL_C_CHAR, rec->DATA_TYPE, DATA_TYPEl, 0);
		ret = com->GetData(9, SQL_C_LONG, &rec->CHARACTER_MAXIMUM_LENGTH, CHARACTER_MAXIMUM_LENGTHl, 0);
		ret = com->GetData(10, SQL_C_LONG, &rec->CHARACTER_OCTET_LENGTH, CHARACTER_OCTET_LENGTHl, 0);
		ret = com->GetData(11, SQL_C_LONG, &rec->NUMERIC_PRECISION, NUMERIC_PRECISIONl, 0);
		ret = com->GetData(12, SQL_C_LONG, &rec->NUMERIC_SCALE, NUMERIC_SCALEl, 0);
		ret = com->GetData(13, SQL_C_LONG, &rec->DATETIME_PRECISION, DATETIME_PRECISIONl, 0);
		ret = com->GetData(14, SQL_C_CHAR, rec->CHARACTER_SET_NAME, CHARACTER_SET_NAMEl, 0);
		ret = com->GetData(15, SQL_C_CHAR, rec->COLLATION_NAME, COLLATION_NAMEl, 0);
		return ret;
	}

public:
	SQLLEN TABLE_CATALOGl = 256;
	SQLLEN TABLE_SCHEMAl = 256;
	SQLLEN TABLE_NAMEl = 256;
	SQLLEN COLUMN_NAMEl = 256;
	SQLLEN ORDINAL_POSITIONl = 4;
	SQLLEN COLUMN_DEFAULTl = 256;
	SQLLEN IS_NULLABLEl = 8;
	SQLLEN DATA_TYPEl = 256;
	SQLLEN CHARACTER_MAXIMUM_LENGTHl = 4;
	SQLLEN CHARACTER_OCTET_LENGTHl = 4;
	SQLLEN NUMERIC_PRECISIONl = 4;
	SQLLEN NUMERIC_SCALEl = 4;
	SQLLEN DATETIME_PRECISIONl = 4;
	SQLLEN CHARACTER_SET_NAMEl = 256;
	SQLLEN COLLATION_NAMEl = 256;

public:
	std::vector<CR_INFORMATION_SCHEMA_COLUMNS> m_Data;

public:
	std::string m_PRIMARY;
};
class CR_INFORMATION_SCHEMA_KEY_COLUMN_USAGE : public COdbcRecord {
public:
	CR_INFORMATION_SCHEMA_KEY_COLUMN_USAGE() : COdbcRecord() {}
	virtual ~CR_INFORMATION_SCHEMA_KEY_COLUMN_USAGE() {}

public:
	SQLCHAR CONSTRAINT_NAME[256];
	SQLCHAR TABLE_CATALOG[256];
	SQLCHAR TABLE_SCHEMA[256];
	SQLCHAR TABLE_NAME[256];
	SQLCHAR COLUMN_NAME[256];
	SQLINTEGER ORDINAL_POSITION;
};
class CT_INFORMATION_SCHEMA_KEY_COLUMN_USAGE : public COdbcTable {
public:
	CT_INFORMATION_SCHEMA_KEY_COLUMN_USAGE() : COdbcTable() {
		m_SqlSELECT = "SELECT CONSTRAINT_NAME, TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME, "
					  "COLUMN_NAME, ORDINAL_POSITION FROM "
					  "INFORMATION_SCHEMA.KEY_COLUMN_USAGE ";
	}
	virtual ~CT_INFORMATION_SCHEMA_KEY_COLUMN_USAGE() {}
	SQLRETURN Set_Data(COdbcCommand *com, CR_INFORMATION_SCHEMA_KEY_COLUMN_USAGE *rec) {
		SQLRETURN ret;
		ret = com->GetData(1, SQL_C_CHAR, rec->CONSTRAINT_NAME, 256, 0);
		ret = com->GetData(2, SQL_C_CHAR, rec->TABLE_CATALOG, 256, 0);
		ret = com->GetData(3, SQL_C_CHAR, rec->TABLE_SCHEMA, 256, 0);
		ret = com->GetData(4, SQL_C_CHAR, rec->TABLE_NAME, 256, 0);
		ret = com->GetData(5, SQL_C_CHAR, rec->COLUMN_NAME, 256, 0);
		ret = com->GetData(6, SQL_C_LONG, &rec->ORDINAL_POSITION, 4, 0);
		return ret;
	}

public:
	std::vector<CR_INFORMATION_SCHEMA_KEY_COLUMN_USAGE> m_Data;
};
namespace fs = std::filesystem;
class CppGen {
public:
	CppGen(std::string filename, std::string ConnectionString, OdbcCommon::COdbcConnection *pconnection,
		   bool UseClangFormat = false);
	virtual ~CppGen();
	int Execute();

protected:
	void OdbcCommonWrite();
	void HeaderWrite(ofstream *outfile);
	void GetKeyColumnUsage(ofstream *ofile, COdbcConnection *con, std::string tablename);
	void WriteRecConstructor(ofstream *outf, std::string classname);
	void WriteRecDestructor(ofstream *outf, std::string classname);
	void WriteRecInitialize(ofstream *outf, CT_INFORMATION_SCHEMA_COLUMNS *tbl);
	void WriteRecordData(ofstream *outf, CT_INFORMATION_SCHEMA_COLUMNS *tbl);
	void WriteRecordOperator(ofstream *outf, CT_INFORMATION_SCHEMA_COLUMNS *tbl);
	void WriteTblConstructor(ofstream *outf, std::string &classname, CT_INFORMATION_SCHEMA_COLUMNS *tbl,
							 std::string &tblname);
	void WriteTblDestructor(ofstream *outf, std::string &classname);
	void WriteSetTableData(ofstream *outf, std::string &classname, CT_INFORMATION_SCHEMA_COLUMNS *tbl);
	void WriteWherePrimaryKey(ofstream *outf, std::string &Recordclassname);
	void WriteSynchronize(ofstream *outf, std::string &Recordclassname, CT_INFORMATION_SCHEMA_COLUMNS *tbl);
	OdbcCommon::eSqlType TypeComparison(std::string &type);
	std::string Get_C_Type(eSqlType typ);

private:
	int FindKey(CR_INFORMATION_SCHEMA_COLUMNS &rec);
	CCharBuffer FindBuffer(std::vector<CCharBuffer> *names, int position);
	std::string m_filename;
	std::string m_common;
	std::string m_ConnectionString;
	std::string m_ServarVarsion;
	int mservertype = 0;
	bool m_ClangFormat;
	OdbcCommon::COdbcConnection *m_con;
	CT_INFORMATION_SCHEMA_KEY_COLUMN_USAGE m_Key;
	std::vector<string> m_Types = {"bit",		"tinyint",	"smallint",	 "int",			  "bigint",			"decimal",
								   "numeric",	"real",		"float",	 "smallmoney",	  "money",			"date",
								   "time",		"datetime", "datetime2", "smalldatetime", "datetimeoffset", "char",
								   "varchar",	"text",		"nchar",	 "nvarchar",	  "ntext",			"binary",
								   "verbinary", "image",	"xml"};
};
#endif
