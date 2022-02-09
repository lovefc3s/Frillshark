#include "CppGen.hpp"
#include "resources/odbccommon.hpp"
#include <algorithm>
#include <cctype>
using namespace std;
using namespace OdbcCommon;
CppGen::CppGen(std::string filename, std::string ConnectionString,
			   OdbcCommon::COdbcConnection *pconnection, bool UseClangFormat) {
	m_filename = filename;
	m_common = "";
	m_ConnectionString = ConnectionString;
	m_ClangFormat = UseClangFormat;
	m_con = pconnection;
	// m_Types = new std::vector<std::string>();
}
CppGen::~CppGen() {}
int CppGen::Execute() {
	SQLRETURN ret;
	ret = m_con->DriverConnect();
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) return ret;
	string sql = "";
	this->OdbcCommonWrite();
	int servertype = 0;
	fs::path p = m_filename;
	OdbcCommon::COdbcCommand *cm1 = new COdbcCommand(m_con);
	sql = "select @@version;";
	cm1->SetCommandString(sql);
	ret = cm1->mSQLExecDirect();
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		sql = "select version();";
		ret = cm1->mSQLExecDirect(sql);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			return SQL_ERROR;
		}
	}
	m_ServarVarsion = "";
	for (int i = 0;; i++) {
		ret = cm1->mFetch();
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			SQLCHAR *var = new SQLCHAR[1024];
			SQLLEN len = 0;
			cm1->GetData(1, SQL_C_CHAR, var, 1024, &len);
			m_ServarVarsion = (char *)var;
			delete[] var;
		} else
			break;
	}
	delete cm1;
	if (m_ServarVarsion.length() < 1) return SQL_ERROR;
	if (m_ServarVarsion.find("Microsoft SQL Server") != std::string::npos) {
		servertype = 1;
	} else if (m_ServarVarsion.find("MySQL") != std::string::npos) {
		servertype = 2;
	} else if (m_ServarVarsion.find("8.0") != std::string::npos) {
		servertype = 2;
	} else if (m_ServarVarsion.find("PostgreSQL") != std::string::npos) {
		servertype = 3;
	}
	if (servertype == 0) return SQL_ERROR;
	cm1 = new COdbcCommand(m_con);
	ofstream *outf = new ofstream(m_filename);
	HeaderWrite(outf);
	*outf << "using namespace OdbcCommon;" << NL;
	sql = "SELECT TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME,"
		  "TABLE_TYPE FROM INFORMATION_SCHEMA.TABLES ";
	switch (servertype) {
	case 1:
		sql = sql + " WHERE TABLE_CATALOG = '" + m_con->Get_Database() + "';";
		break;
	case 2:
		sql = sql + " WHERE TABLE_SCHEMA = '" + m_con->Get_Database() + "';";
		break;
	case 3:
		sql = sql + " WHERE TABLE_SCHEMA = 'public' AND TABLE_CATALOG = '" +
			  m_con->Get_Database() + "';";
		break;
	default:
		return SQL_ERROR;
	}

	ret = cm1->mSQLExecDirect(sql);
	string tblname = "";
	vector<string> *vtbl = new vector<string>();
	for (int i = 0;; i++) {
		ret = cm1->mFetch();
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			SQLCHAR *tbl = new SQLCHAR[1024];
			SQLLEN len = 0;
			cm1->GetData(3, SQL_C_CHAR, tbl, 1024, &len);
			tblname = (char *)tbl;
			vtbl->push_back(tblname);
			delete[] tbl;
		} else
			break;
	}
	delete cm1;
	if (vtbl->size() < 1) {
		delete vtbl;
		return SQL_NO_DATA;
	}
	for (int i = 0; i < vtbl->size(); i++) {
		tblname = vtbl->at(i);
		COdbcCommand *cm2 = new COdbcCommand(m_con);
		sql = "SELECT TABLE_CATALOG, TABLE_SCHEMA, TABLE_NAME, COLUMN_NAME, "
			  "ORDINAL_POSITION, "
			  "COLUMN_DEFAULT, IS_NULLABLE, DATA_TYPE, "
			  "CHARACTER_MAXIMUM_LENGTH, CHARACTER_OCTET_LENGTH, "
			  "NUMERIC_PRECISION, NUMERIC_SCALE, DATETIME_PRECISION, "
			  "CHARACTER_SET_NAME, COLLATION_NAME "
			  "FROM INFORMATION_SCHEMA.COLUMNS ";
		if (servertype == 2) {
			sql = sql + " WHERE TABLE_SCHEMA = '" + m_con->Get_Database() +
				  "' AND TABLE_NAME = '" + tblname +
				  "' ORDER BY ORDINAL_POSITION;";
		} else {
			sql = sql + " WHERE TABLE_CATALOG = '" + m_con->Get_Database() +
				  "' AND TABLE_NAME = '" + tblname +
				  "' ORDER BY ORDINAL_POSITION;";
		}
		string RecClassName = "CR_" + tblname;
		string TblClassName = "CT_" + tblname;
		*outf << "class " << RecClassName << ":public COdbcRecord {" << NL;
		*outf << "public:" << NL;
		WriteRecConstructor(outf, RecClassName);
		WriteRecDestructor(outf, RecClassName);
		CT_INFORMATION_SCHEMA_COLUMNS *tbl =
			new CT_INFORMATION_SCHEMA_COLUMNS();
		ret = cm2->mSQLExecDirect(sql);
		for (int j = 0;; j++) {
			ret = cm2->mFetch();
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				CR_INFORMATION_SCHEMA_COLUMNS rec;
				tbl->Set_Data(cm2, &rec);
				string type = (char *)rec.DATA_TYPE;
				rec.sqltype = TypeComparison(type);
				tbl->m_Data.push_back(rec);
			} else
				break;
		}
		WriteRecInitialize(outf, tbl);
		WriteRecordData(outf, tbl);
		*outf << "};" << NL;
		*outf << "class " << TblClassName << ":public COdbcTable {" << NL;
		*outf << "public:" << NL;
		WriteTblConstructor(outf, TblClassName, tbl, tblname);
		WriteTblDestructor(outf, TblClassName);
		*outf << Tab << RecClassName << " operator[](int n) {" << NL;
		*outf << Tab << Tab << "return m_Data.at(n);" << NL;
		*outf << Tab << "}" << NL;
		*outf << "public:" << NL;
		*outf << Tab << "std::vector<" << RecClassName << "> m_Data;";
		*outf << "};" << NL;
		delete tbl;
		delete cm2;
	}
	ret = m_con->Disconnect();
	delete vtbl;
	*outf << "#endif" << NL;
	outf->close();
	delete outf;
	if (m_ClangFormat) {
		std::string shellcom = "clang-format -i " + m_filename;
		system(shellcom.c_str());
	}
	return SQL_SUCCESS;
}
void CppGen::OdbcCommonWrite() {
	fs::path p = m_filename;
	fs::path dir = p.parent_path();
	fs::path res = "./resources/odbccommon.hpp";
	m_common = "odbccommon_" + p.stem().native() + ".hpp";
	string afxname = dir.native() + "/odbccommon_" + p.stem().native() + ".hpp";
	ofstream file_out;
	ifstream file_in(res.native());
	if (!file_in) {
		return;
	}

	file_out.open(afxname);
	file_out << "/*" << NL;
	file_out << Tab << "This file FrillShark Odbc C++ source Generation." << NL;
	file_out << "*/" << NL;

	char c;
	while (file_out && file_in.get(c)) {
		file_out.put(c);
	}
	file_out << NL;
	file_out.close();
	file_in.close();
}
void CppGen::HeaderWrite(ofstream *ofile) {
	fs::path p = m_filename;
	*ofile << "/*" << NL;
	*ofile << Tab << "This file FrillShark Odbc C++ source Generation." << NL;
	*ofile << Tab << p.stem() << p.extension() << NL;
	*ofile << Tab << "Set_Driver(\"" << m_con->Get_Driver() << "\");" << NL;
	*ofile << Tab << "Set_Server(\"" << m_con->Get_Server() << "\");" << NL;
	*ofile << Tab << "Set_UserID(\"" << m_con->Get_UserID() << "\");" << NL;
	*ofile << Tab << "Set_Password(\" .... \");" << NL;
	*ofile << Tab << "Set_Database(\"" << m_con->Get_Database() << "\");" << NL;
	*ofile << Tab << m_ServarVarsion << NL;
	*ofile << "*/" << NL;
	*ofile << NL;
	const std::string under = "_";
	std::string destination = "";
	std::string source = p.stem().native() + p.extension().native();
	destination.resize(source.size());
	// std::transform(source.cbegin(), source.cend(),
	// destination.begin(),toupper);
	char buf[256];
	memset(buf, 0, 256);
	for (int j = 0; j < source.length(); j++) {
		buf[j] = (char)toupper((int)source.at(j));
	}
	destination = buf;
	size_t pos = destination.find('.');
	if (pos != std::string::npos) {
		destination.replace(pos, 1, under);
	}
	*ofile << "#ifndef __" << destination << "__" << NL;
	*ofile << "#define __" << destination << "__" << NL;
	*ofile << "#include \"" << m_common << "\"" << NL;
}
void CppGen::WriteRecConstructor(ofstream *outf, std::string classname) {
	*outf << Tab << classname << "():COdbcRecord() { " << NL;
	*outf << Tab << Tab << "Initialize();" << NL;
	*outf << Tab << "}" << NL;
}
void CppGen::WriteRecDestructor(ofstream *outf, std::string classname) {
	*outf << Tab << "virtual ~" << classname << "() { " << NL;
	*outf << Tab << "}" << NL;
	*outf << NL;
}
void CppGen::WriteRecInitialize(ofstream *outf,
								CT_INFORMATION_SCHEMA_COLUMNS *tbl) {
	*outf << Tab << "void Initialize() { " << NL;
	for (int i = 0; i < tbl->m_Data.size(); i++) {
		CR_INFORMATION_SCHEMA_COLUMNS rec = tbl->m_Data.at(i);
		switch (rec.sqltype) {
		case _unknown:
			break;
		case _bit:
		case _tinyint:
		case _smallint:
		case _int:
		case _bigint: {
			*outf << Tab << Tab << rec.COLUMN_NAME << Tab << "= 0;" << NL;
			break;
		}
		case _real:
		case _float: {
			*outf << Tab << Tab << rec.COLUMN_NAME << Tab << "= 0.0;" << NL;
			break;
		}
		case _date:
		case _time:
		case _datetime:
		case _datetime2:
		case _smalldatetime:
		case _datetimeoffset:
			break;
		case _decimal:
		case _numeric:
		case _smallmoney:
		case _money: {
			*outf << Tab << Tab << "memset(" << rec.COLUMN_NAME << ",0,48);"
				  << NL;
			break;
		}
		case _char:
		case _varchar:
		case _text:
		case _nchar:
		case _nvarchar:
		case _ntext: {
			*outf << Tab << Tab << rec.COLUMN_NAME << Tab << "= \"\";" << NL;
			break;
		}
		case _binary: {
			*outf << Tab << Tab << "memset(" << rec.COLUMN_NAME << ",0,"
				  << rec.CHARACTER_MAXIMUM_LENGTH << ");" << NL;
			break;
		}
		default:
			break;
		}
	}
	*outf << Tab << "}" << NL;
}
OdbcCommon::eSqlType CppGen::TypeComparison(std::string &type) {
	for (int n = 0; n < m_Types.size(); n++) {
		if (type == m_Types.at(n)) return (OdbcCommon::eSqlType)n;
	}
	return eSqlType::_unknown;
}
void CppGen::WriteRecordData(ofstream *outf,
							 CT_INFORMATION_SCHEMA_COLUMNS *tbl) {
	int cnt = 0;
	*outf << "public:" << NL;
	for (int i = 0; i < tbl->m_Data.size(); i++) {
		CR_INFORMATION_SCHEMA_COLUMNS rec = tbl->m_Data.at(i);
		switch (rec.sqltype) {
		case _unknown:
			break;
		case _bit:
		case _tinyint: {
			*outf << Tab << "SQLCHAR" << Tab << rec.COLUMN_NAME << ";" << NL;
			break;
		}
		case _smallint: {
			*outf << Tab << "SQLSMALLINT" << Tab << rec.COLUMN_NAME << ";"
				  << NL;
			break;
		}
		case _int: {
			*outf << Tab << "SQLINTEGER" << Tab << rec.COLUMN_NAME << ";" << NL;
			break;
		}
		// long long
		case _bigint: {
			*outf << Tab << "SQLLEN" << Tab << rec.COLUMN_NAME << ";" << NL;
			break;
		}
		case _decimal:
		case _numeric:
		case _smallmoney:
		case _money: {
			*outf << Tab << "SQLCHAR" << Tab << rec.COLUMN_NAME << "[48];"
				  << NL;
			break;
		}
		case _real:
		case _float: {
			*outf << Tab << "SQLFLOAT" << Tab << rec.COLUMN_NAME << ";" << NL;
			break;
		}
		case _date:
		case _time:
		case _datetime:
		case _datetime2:
		case _smalldatetime:
		case _datetimeoffset: {
			*outf << Tab << "TIMESTAMP_STRUCT" << Tab << rec.COLUMN_NAME << ";"
				  << NL;
			break;
		}
		case _char:
		case _varchar:
		case _text:
		case _nchar:
		case _nvarchar:
		case _ntext: {
			*outf << Tab << "std::string" << Tab << rec.COLUMN_NAME << ";"
				  << NL;
			break;
		}
		case _binary: {
			int len = MAXBUF;
			rec.CHARACTER_MAXIMUM_LENGTH;
			int wklen = (int)rec.CHARACTER_MAXIMUM_LENGTH;
			if (wklen < 0) wklen = MAXBUF;
			if (wklen > MAXBUF)
				len = MAXBUF;
			else
				len = wklen;
			*outf << Tab << "SQLCHAR" << Tab << rec.COLUMN_NAME << "[" << len
				  << "];" << NL;
			break;
		}
		default:
			break;
		}
		cnt++;
	}
}
void CppGen::WriteTblConstructor(ofstream *outf, std::string &classname,
								 CT_INFORMATION_SCHEMA_COLUMNS *tbl,
								 std::string &tblname) {
	*outf << Tab << classname << "():COdbcTable() { " << NL;
	*outf << Tab << Tab << "m_TableName = \"" << tblname << "\";" << NL;
	*outf << Tab << Tab << "m_SqlSELECT = \"SELECT \"" << NL;
	std::stringstream insrt;
	insrt << Tab << Tab << "m_SqlINSERT = \"INSERT INTO " << tblname << " (\""
		  << NL;
	std::stringstream updat;
	updat << Tab << Tab << "m_SqlUPDATE = \"UPDATE " << tblname << " SET \""
		  << NL;
	for (int i = 0; i < tbl->m_Data.size(); i++) {
		CR_INFORMATION_SCHEMA_COLUMNS rec = tbl->m_Data.at(i);
		if (i == (tbl->m_Data.size() - 1)) {
			*outf << Tab << Tab << Tab << "\"" << rec.COLUMN_NAME << "\"" << NL;
			*outf << Tab << Tab << Tab << "\" FROM " << rec.TABLE_NAME << "\";"
				  << NL;
			insrt << Tab << Tab << Tab << "\"" << rec.COLUMN_NAME
				  << ") VALUES \"" << NL;
			insrt << Tab << Tab << Tab << "\"( ";
			for (int j = 0; j < tbl->m_Data.size(); j++) {
				if (j == (tbl->m_Data.size() - 1)) {
					insrt << "? )\";";
				} else {
					insrt << "?,";
				}
			}
			updat << Tab << Tab << Tab << "\"" << rec.COLUMN_NAME << " = ?\";"
				  << NL;
		} else {
			*outf << Tab << Tab << Tab << "\"" << rec.COLUMN_NAME << ",\""
				  << NL;
			insrt << Tab << Tab << Tab << "\"" << rec.COLUMN_NAME << ",\""
				  << NL;
			updat << Tab << Tab << Tab << "\"" << rec.COLUMN_NAME << " = ?,\""
				  << NL;
		}
	}
	*outf << insrt.str() << NL;
	*outf << updat.str() << NL;
	*outf << Tab << Tab << "m_SqlDELETE = \"DELETE " << tblname << " \";" << NL;
	*outf << Tab << Tab << "COdbcColumn col;" << NL;
	for (int j = 0; j < tbl->m_Data.size(); j++) {
		CR_INFORMATION_SCHEMA_COLUMNS rec = tbl->m_Data.at(j);
		*outf << Tab << Tab << "col.SetValue(\"" << rec.TABLE_CATALOG << "\",\""
			  << rec.TABLE_SCHEMA << "\",\"" << rec.TABLE_NAME << "\",\""
			  << rec.COLUMN_NAME << "\",\"" << rec.ORDINAL_POSITION << "\",\""
			  << rec.COLUMN_DEFAULT << "\",\"" << rec.IS_NULLABLE << "\",\""
			  << rec.DATA_TYPE << "\",\"" << rec.CHARACTER_MAXIMUM_LENGTH
			  << "\",\"" << rec.CHARACTER_OCTET_LENGTH << "\",\""
			  << rec.NUMERIC_PRECISION << "\",\"" << rec.NUMERIC_SCALE
			  << "\",\"" << rec.DATETIME_PRECISION << "\",\""
			  << rec.CHARACTER_SET_NAME << "\",\"" << rec.COLLATION_NAME
			  << "\",";
		string stype = "";
		switch (rec.sqltype) {
		case _unknown:
			stype = "_unknown";
			break;
		case _bit:
			stype = "_bit";
			break;
		case _tinyint:
			stype = "_tinyint";
			break;
		case _smallint:
			stype = "_smallint";
			break;
		case _int:
			stype = "_int";
			break;
		// long long
		case _bigint:
			stype = "_bigint";
			break;
		case _decimal:
			stype = "_decimal";
			break;
		case _numeric:
			stype = "_numeric";
			break;
		case _real:
			stype = "_real";
			break;
		case _float:
			stype = "_float";
			break;
		case _smallmoney:
			stype = "_smallmoney";
			break;
		case _money:
			stype = "_money";
			break;
		case _date:
			stype = "_date";
			break;
		case _time:
			stype = "_time";
			break;
		case _datetime:
			stype = "_datetime";
			break;
		case _datetime2:
			stype = "_datetime2";
			break;
		case _smalldatetime:
			stype = "_smalldatetime";
			break;
		case _datetimeoffset:
			stype = "_datetimeoffset";
			break;
		case _char:
			stype = "_char";
			break;
		case _varchar:
			stype = "_varchar";
			break;
		case _text:
			stype = "_text";
			break;
		case _nchar:
			stype = "_nchar";
			break;
		case _nvarchar:
			stype = "_nvarchar";
			break;
		case _ntext:
			stype = "_ntext";
			break;
		default:
			stype = "_unknown";
			break;
		}
		*outf << stype << ");" << NL;
		*outf << Tab << Tab << "m_Column.push_back(col);" << NL;
	}

	*outf << Tab << "}" << NL;
}
void CppGen::WriteTblDestructor(ofstream *outf, std::string &classname) {
	*outf << Tab << "virtual ~" << classname << "() { " << NL;
	*outf << Tab << Tab << "m_Data.clear();" << NL;
	*outf << Tab << "}" << NL;
	*outf << NL;
}
