#include "testdb.hpp"
using namespace OdbcCommon;
int main(int argc, char *argv[]) {
	COdbcConnection con("ODBC Driver 17 for SQL Server", "192.168.1.16", "testuser", "010101", "testdb");
	// con.Set_Driver("ODBC Driver 17 for SQL Server");
	// con.Set_Server("192.168.1.16");
	// con.Set_UserID("testuser");
	// con.Set_Password("010101");
	// con.Set_Database("testdb");
	SQLRETURN ret = con.DriverConnect();
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cout << "Connection to server failed. Error:" << ret << std::endl;
		return ret;
	}
	COdbcCommand com(&con);
	std::string sql = "SELECT ";
	std::stringstream ss;
	CT_t_testtable tbl;
	ss << sql;
	for (int i = 0; i < tbl.ColumnCount(); i++) {
		COdbcColumn column = tbl.Column(i);
		ss << column.column_name;
		if (i != (tbl.ColumnCount() - 1)) ss << ", ";
	}
	ss << " FROM " << tbl.Get_Name();
	sql = ss.str();
	com.SetCommandString(sql);
	com.mSQLExecDirect();
	SQLCHAR *name = new SQLCHAR[MAXBUF];
	for (int i = 0;; i++) {
		ret = com.mFetch();
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			CR_t_testtable rec;
			SQLLEN len = 0;
			memset(name, 0, MAXBUF);
			com.GetData(1, SQL_C_LONG, &rec.id, 4, 0);
			com.GetData(2, SQL_C_CHAR, name, MAXBUF, &len);
			rec.name = (char *)name;
			tbl.m_Data.push_back(rec);
		} else
			break;
	}
	delete[] name;

	con.Disconnect();
	return 0;
}
