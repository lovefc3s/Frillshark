#include "testdb.hpp"
using namespace OdbcCommon;
int main(int argc, char *argv[]) {
	COdbcConnection con("ODBC Driver 17 for SQL Server", "192.168.0.254", "testuser", "010101", "testdb");
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
	CT_t_testtable tbl;
	
	SQLLEN cnt = tbl.Set_TableData(&com);
	for (SQLLEN i = 0; i < cnt; i++) {
		CR_t_testtable rec = tbl[i];
	}
	con.Disconnect();
	return 0;
}
