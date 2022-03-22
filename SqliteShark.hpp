#ifndef __SQLITESHARK_HPP__
#define __SQLITESHARK_HPP__
//#include <sqlite3.h>
//#include <string>
namespace Shark {
// SQLITE_EXTENSION_INIT1

class SqliteConnection {
private:
	std::string _ConnectionString;
	sqlite3 *_Db;
	int _status;

	// const sqlite3_api_routines api;
public:
	SqliteConnection(/* args */) {
		//      sqlite3_api = new sqlite3_api_routines();
		_Db = nullptr;
		_status = -1;
	}
	SqliteConnection(std::string connection) {
		_ConnectionString = connection;
		_Db = nullptr;
		_status = -1;
	}
	virtual ~SqliteConnection() {}
	std::string getConnectionString() { return _ConnectionString; }
	sqlite3 *getDb() { return _Db; }
	int Open() {
		_status = sqlite3_open(_ConnectionString.c_str(), &_Db);
		return _status;
	}
	int Open(std::string ConnectionString) {
		_ConnectionString = ConnectionString;
		return Open();
	}
	void Close() { sqlite3_close(_Db); }
};
class SqliteCommand {
private:
	sqlite3 *_Db;
	SqliteConnection *_con;
	sqlite3_stmt *_pstmt;
	std::string _sql;
	int _prepare;
	int _stmt;

protected:
	// 抽出結果が返るコールバック関数
	static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
		int i;
		for (i = 0; i < argc; i++)
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		return SQLITE_OK;
	}

public:
	SqliteCommand(SqliteConnection *DB) {
		_con = DB;
		_sql = "";
		_Db = DB->getDb();
		_prepare = -1;
		_stmt = -1;
		_pstmt = nullptr;
	}
	virtual ~SqliteCommand() {
		if (_pstmt != nullptr) {
			Reset();
			Finalize();
		}
	}
	void setSql(std::string sql) { _sql = sql; }
	int Prepare(/*sqlite3 *db,*/ const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) {
		int rc = -1;
		rc = sqlite3_prepare_v2(_Db, zSql, nByte, ppStmt, pzTail);
		_pstmt = *ppStmt;
		return rc;
	}
	int Step() { return sqlite3_step(_pstmt); }
	int Step(sqlite3_stmt *statement) {
		_pstmt = statement;
		return sqlite3_step(statement);
	}
	int Column_int(int column) { return sqlite3_column_int(_pstmt, column); }
	int Column_int(sqlite3_stmt *statement, int column) {
		_pstmt = statement;
		return Column_int(column);
	}
	const unsigned char *Column_text(int column) { return sqlite3_column_text(_pstmt, column); }
	const unsigned char *Column_text(sqlite3_stmt *statement, int column) {
		_pstmt = statement;
		return Column_text(column);
	}
	double Column_double(int column) { return sqlite3_column_double(_pstmt, column); }
	double Column_double(sqlite3_stmt *statement, int column) {
		_pstmt = statement;
		return Column_double(column);
	}
	void Reset() { sqlite3_reset(_pstmt); }
	void Reset(sqlite3_stmt *statement) { sqlite3_reset(statement); }
	void Finalize() {
		sqlite3_finalize(_pstmt);
		_pstmt = nullptr;
	}
	void Finalize(sqlite3_stmt *statement) { sqlite3_finalize(statement); }
	int Exec(char *errmsg) {
		int rc = -1;
		rc = sqlite3_exec(_Db, _sql.c_str(), 0, 0, &errmsg);
		return rc;
	}
};
} // namespace Shark
#endif //__SQLITESHARK_HPP__
