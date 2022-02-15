/*
	This file FrillShark Odbc C++ source Generation.
*/
#ifndef ODBCCOMMON_HPP_INCLUDED
#define ODBCCOMMON_HPP_INCLUDED
#ifdef _WIN32
#include <windows.h>
#endif
#include <array>
#include <cmath>
#include <codecvt>
#include <cstdio>
#include <cstring>
#include <cuchar>
#include <cwchar>
#include <iconv.h>
#include <iostream>
#include <locale>
#include <sql.h>
#include <sqlext.h>
#include <sstream>
#include <string>
#include <vector>
//#include <msclr/marshal_cppstd.h>
//#include <emmintrin.h>
#define MAXBUF 1048576

using namespace std;
union my__128 {
	unsigned long long x[2];
	unsigned char y[16];
#ifndef _WIN32
	unsigned __int128 z;
#endif
};
namespace OdbcCommon {
typedef enum {
	_unknown = -1,
	_bit = 0,
	_tinyint = 1,
	_smallint = 2,
	_int = 3,
	_bigint = 4,
	_decimal = 5,
	_numeric = 6,
	_real = 7,
	_float = 8,
	_smallmoney = 9,
	_money = 10,
	_date = 11,
	_time = 12,
	_datetime = 13,
	_datetime2 = 14,
	_smalldatetime = 15,
	_datetimeoffset = 16,
	_char = 17,
	_varchar = 18,
	_text = 19,
	_nchar = 20,
	_nvarchar = 21,
	_ntext = 22,
	_binary = 23,
	_verbinary = 24,
	_image = 25,
	_xml = 26,
} eSqlType;

typedef enum {
	_NoModify = 0,
	_Select = 1,
	_Insert = 2,
	_Update = 3,
	_Delete = 4,
} eRecordModify;

class UtfConvartor {
public:
	//
	// utf32 to utf8
	//
	static bool ConvChU32ToU8(const char32_t u32Ch, std::array<char, 4> &u8Ch) {
		if (u32Ch < 0 || u32Ch > 0x10FFFF) {
			return false;
		}

		if (u32Ch < 128) {
			u8Ch[0] = char(u32Ch);
			u8Ch[1] = 0;
			u8Ch[2] = 0;
			u8Ch[3] = 0;
		} else if (u32Ch < 2048) {
			u8Ch[0] = 0xC0 | char(u32Ch >> 6);
			u8Ch[1] = 0x80 | (char(u32Ch) & 0x3F);
			u8Ch[2] = 0;
			u8Ch[3] = 0;
		} else if (u32Ch < 65536) {
			u8Ch[0] = 0xE0 | char(u32Ch >> 12);
			u8Ch[1] = 0x80 | (char(u32Ch >> 6) & 0x3F);
			u8Ch[2] = 0x80 | (char(u32Ch) & 0x3F);
			u8Ch[3] = 0;
		} else {
			u8Ch[0] = 0xF0 | char(u32Ch >> 18);
			u8Ch[1] = 0x80 | (char(u32Ch >> 12) & 0x3F);
			u8Ch[2] = 0x80 | (char(u32Ch >> 6) & 0x3F);
			u8Ch[3] = 0x80 | (char(u32Ch) & 0x3F);
		}

		return true;
	}

	// UTF-8 から UTF-32

	static int GetU8ByteCount(char ch) {
		if (0 <= uint8_t(ch) && uint8_t(ch) < 0x80) {
			return 1;
		}
		if (0xC2 <= uint8_t(ch) && uint8_t(ch) < 0xE0) {
			return 2;
		}
		if (0xE0 <= uint8_t(ch) && uint8_t(ch) < 0xF0) {
			return 3;
		}
		if (0xF0 <= uint8_t(ch) && uint8_t(ch) < 0xF8) {
			return 4;
		}
		return 0;
	}

	static bool IsU8LaterByte(char ch) {
		return 0x80 <= uint8_t(ch) && uint8_t(ch) < 0xC0;
	}

	static bool ConvChU8ToU32(const std::array<char, 4> &u8Ch,
							  char32_t &u32Ch) {
		int numBytes = GetU8ByteCount(u8Ch[0]);
		if (numBytes == 0) {
			return false;
		}
		switch (numBytes) {
		case 1:
			u32Ch = char32_t(uint8_t(u8Ch[0]));
			break;
		case 2:
			if (!IsU8LaterByte(u8Ch[1])) {
				return false;
			}
			if ((uint8_t(u8Ch[0]) & 0x1E) == 0) {
				return false;
			}

			u32Ch = char32_t(u8Ch[0] & 0x1F) << 6;
			u32Ch |= char32_t(u8Ch[1] & 0x3F);
			break;
		case 3:
			if (!IsU8LaterByte(u8Ch[1]) || !IsU8LaterByte(u8Ch[2])) {
				return false;
			}
			if ((uint8_t(u8Ch[0]) & 0x0F) == 0 &&
				(uint8_t(u8Ch[1]) & 0x20) == 0) {
				return false;
			}

			u32Ch = char32_t(u8Ch[0] & 0x0F) << 12;
			u32Ch |= char32_t(u8Ch[1] & 0x3F) << 6;
			u32Ch |= char32_t(u8Ch[2] & 0x3F);
			break;
		case 4:
			if (!IsU8LaterByte(u8Ch[1]) || !IsU8LaterByte(u8Ch[2]) ||
				!IsU8LaterByte(u8Ch[3])) {
				return false;
			}
			if ((uint8_t(u8Ch[0]) & 0x07) == 0 &&
				(uint8_t(u8Ch[1]) & 0x30) == 0) {
				return false;
			}

			u32Ch = char32_t(u8Ch[0] & 0x07) << 18;
			u32Ch |= char32_t(u8Ch[1] & 0x3F) << 12;
			u32Ch |= char32_t(u8Ch[2] & 0x3F) << 6;
			u32Ch |= char32_t(u8Ch[3] & 0x3F);
			break;
		default:
			return false;
		}

		return true;
	}
	//
	// UTF-16
	// UTF-32 から UTF-16
	//
	static bool ConvChU32ToU16(const char32_t u32Ch,
							   std::array<char16_t, 2> &u16Ch) {
		if (u32Ch < 0 || u32Ch > 0x10FFFF) {
			return false;
		}

		if (u32Ch < 0x10000) {
			u16Ch[0] = char16_t(u32Ch);
			u16Ch[1] = 0;
		} else {
			u16Ch[0] = char16_t((u32Ch - 0x10000) / 0x400 + 0xD800);
			u16Ch[1] = char16_t((u32Ch - 0x10000) % 0x400 + 0xDC00);
		}

		return true;
	}
	//
	// UTF-16 から UTF-32
	//
	static bool IsU16HighSurrogate(char16_t ch) {
		return 0xD800 <= ch && ch < 0xDC00;
	}

	static bool IsU16LowSurrogate(char16_t ch) {
		return 0xDC00 <= ch && ch < 0xE000;
	}

	static bool ConvChU16ToU32(const std::array<char16_t, 2> &u16Ch,
							   char32_t &u32Ch) {
		if (IsU16HighSurrogate(u16Ch[0])) {
			if (IsU16LowSurrogate(u16Ch[1])) {
				u32Ch = 0x10000 + (char32_t(u16Ch[0]) - 0xD800) * 0x400 +
						(char32_t(u16Ch[1]) - 0xDC00);
			} else if (u16Ch[1] == 0) {
				u32Ch = u16Ch[0];
			} else {
				return false;
			}
		} else if (IsU16LowSurrogate(u16Ch[0])) {
			if (u16Ch[1] == 0) {
				u32Ch = u16Ch[0];
			} else {
				return false;
			}
		} else {
			u32Ch = u16Ch[0];
		}

		return true;
	}
	//
	// UTF-8 から UTF-16 または UTF-16 から UTF-8
	//
	//一度 UTF-32 に変換し、UTF-8、UTF-16 へ変換します。
	// UTF-8 から UTF-16
	//
	static bool ConvChU8ToU16(const std::array<char, 4> &u8Ch,
							  std::array<char16_t, 2> &u16Ch) {
		char32_t u32Ch;
		if (!ConvChU8ToU32(u8Ch, u32Ch)) {
			return false;
		}
		if (!ConvChU32ToU16(u32Ch, u16Ch)) {
			return false;
		}
		return true;
	}
	//
	// UTF-16 から UTF-8
	//
	static bool ConvChU16ToU8(const std::array<char16_t, 2> &u16Ch,
							  std::array<char, 4> &u8Ch) {
		char32_t u32Ch;
		if (!ConvChU16ToU32(u16Ch, u32Ch)) {
			return false;
		}
		if (!ConvChU32ToU8(u32Ch, u8Ch)) {
			return false;
		}
		return true;
	}
	//
	//文字列
	//
	//後は文字列の各文字を変換していきます。
	//
	static bool ConvU8ToU16(const std::string &u8Str, std::u16string &u16Str) {
		for (auto u8It = u8Str.begin(); u8It != u8Str.end(); ++u8It) {
			auto numBytes = GetU8ByteCount((*u8It));
			if (numBytes == 0) {
				return false;
			}

			std::array<char, 4> u8Ch;
			u8Ch[0] = (*u8It);
			for (int i = 1; i < numBytes; i++) {
				++u8It;
				if (u8It == u8Str.end()) {
					return false;
				}
				u8Ch[i] = (*u8It);
			}

			std::array<char16_t, 2> u16Ch;
			if (!ConvChU8ToU16(u8Ch, u16Ch)) {
				return false;
			}

			u16Str.push_back(u16Ch[0]);
			if (u16Ch[1] != 0) {
				u16Str.push_back(u16Ch[1]);
			}
		}
		return true;
	}

	static bool ConvU8ToU32(const std::string &u8Str, std::u32string &u32Str) {
		for (auto u8It = u8Str.begin(); u8It != u8Str.end(); ++u8It) {
			auto numBytes = GetU8ByteCount((*u8It));
			if (numBytes == 0) {
				return false;
			}

			std::array<char, 4> u8Ch;
			u8Ch[0] = (*u8It);
			for (int i = 1; i < numBytes; i++) {
				++u8It;
				if (u8It == u8Str.end()) {
					return false;
				}
				u8Ch[i] = (*u8It);
			}

			char32_t u32Ch;
			if (!ConvChU8ToU32(u8Ch, u32Ch)) {
				return false;
			}

			u32Str.push_back(u32Ch);
		}
		return true;
	}

	static bool ConvU16ToU8(const std::u16string &u16Str, std::string &u8Str) {
		for (auto u16It = u16Str.begin(); u16It != u16Str.end(); ++u16It) {
			std::array<char16_t, 2> u16Ch;
			if (IsU16HighSurrogate((*u16It))) {
				u16Ch[0] = (*u16It);
				++u16It;
				if (u16It == u16Str.end()) {
					return false;
				}
				u16Ch[1] = (*u16It);
			} else {
				u16Ch[0] = (*u16It);
				u16Ch[1] = 0;
			}

			std::array<char, 4> u8Ch;
			if (!ConvChU16ToU8(u16Ch, u8Ch)) {
				return false;
			}
			if (u8Ch[0] != 0) {
				u8Str.push_back(u8Ch[0]);
			}
			if (u8Ch[1] != 0) {
				u8Str.push_back(u8Ch[1]);
			}
			if (u8Ch[2] != 0) {
				u8Str.push_back(u8Ch[2]);
			}
			if (u8Ch[3] != 0) {
				u8Str.push_back(u8Ch[3]);
			}
		}
		return true;
	}

	static bool ConvU16ToU32(const std::u16string &u16Str,
							 std::u32string &u32Str) {
		for (auto u16It = u16Str.begin(); u16It != u16Str.end(); ++u16It) {
			std::array<char16_t, 2> u16Ch;
			if (IsU16HighSurrogate((*u16It))) {
				u16Ch[0] = (*u16It);
				++u16It;
				if (u16It == u16Str.end()) {
					return false;
				}
				u16Ch[1] = (*u16It);
			} else {
				u16Ch[0] = (*u16It);
				u16Ch[1] = 0;
			}

			char32_t u32Ch;
			if (!ConvChU16ToU32(u16Ch, u32Ch)) {
				return false;
			}
			u32Str.push_back(u32Ch);
		}
		return true;
	}

	static bool ConvU32ToU8(const std::u32string &u32Str, std::string &u8Str) {
		for (auto u32It = u32Str.begin(); u32It != u32Str.end(); ++u32It) {
			std::array<char, 4> u8Ch;
			if (!ConvChU32ToU8((*u32It), u8Ch)) {
				return false;
			}

			if (u8Ch[0] != 0) {
				u8Str.push_back(u8Ch[0]);
			}
			if (u8Ch[1] != 0) {
				u8Str.push_back(u8Ch[1]);
			}

			if (u8Ch[2] != 0) {
				u8Str.push_back(u8Ch[2]);
			}
			if (u8Ch[3] != 0) {
				u8Str.push_back(u8Ch[3]);
			}
		}
		return true;
	}

	static bool ConvU32ToU16(const std::u32string &u32Str,
							 std::u16string &u16Str) {
		for (auto u32It = u32Str.begin(); u32It != u32Str.end(); ++u32It) {
			std::array<char16_t, 2> u16Ch;
			if (!ConvChU32ToU16((*u32It), u16Ch)) {
				return false;
			}

			if (u16Ch[0] != 0) {
				u16Str.push_back(u16Ch[0]);
			}
			if (u16Ch[1] != 0) {
				u16Str.push_back(u16Ch[1]);
			}
		}
		return true;
	}

	//
	// utf16 --> utf8
	//
	static bool UtfConv(char *pout, char16_t *pin) {
		bool ret = false;
		mbstate_t st = {0};
		size_t siz;
		char16_t *ptr_string = pin;
		if (setlocale(LC_CTYPE, "") == NULL) { //"ja_JP") == NULL) {
			// fputs( "ロケールの設定に失敗しました。\n", stderr );
			return ret;
		}
		while (*ptr_string) {
			char16_t character = *ptr_string;
			siz = c16rtomb(pout, character, &st);
			if (siz == (size_t)-1)
				break;
			else
				ret = true;
			pout += siz;
			ptr_string++;
		}
		return ret;
	}
};

class COdbcDateTime {
public:
	COdbcDateTime() { Initialize(); }
	COdbcDateTime(TIMESTAMP_STRUCT *ptr) { data = *ptr; }
	virtual ~COdbcDateTime() {}
	static void Initialize(TIMESTAMP_STRUCT *ptr) {
		if (ptr) {
			ptr->year = 0;
			ptr->month = 0;
			ptr->day = 0;
			ptr->hour = 0;
			ptr->minute = 0;
			ptr->second = 0;
			ptr->fraction = 0;
		}
	}
	void Initialize() { this->Initialize(&data); }
	static std::string to_string(SQL_TIMESTAMP_STRUCT *ptr, char separator,
								 bool time) {
		std::stringstream ss;
		ss << ptr->year << separator << ptr->month << separator << ptr->day;
		if (time) {
			ss << " " << ptr->hour << ":" << ptr->minute << ":" << ptr->second
			   << "." << ptr->fraction;
		}
		std::string ret = ss.str();
		return ret;
	}
	std::string to_string(char separator, bool time) {
		return to_string(&data, separator, time);
	}
	std::string to_string() { return to_string(&data, '-', false); }
	static void stringtodate(TIMESTAMP_STRUCT *ptr, std::string &strtim,
							 std::string strsep = "-") {
		Initialize(ptr);
		size_t s1 = strtim.find(strsep);
		std::string yy = strtim.substr(0, s1);
		ptr->year = atoi(yy.c_str());
		size_t s2 = strtim.find(strsep, s1 + 1);
		std::string mm = strtim.substr(s1 + 1, s2 - (s1 + 1));
		ptr->month = atoi(mm.c_str());
		size_t s3 = strtim.find(" ", s2 + 1);
		if (s3 == std::string::npos) s3 = strtim.length();
		std::string dd = strtim.substr(s2 + 1, s3 - (s2 + 1));
		ptr->day = atoi(dd.c_str());
	}
	void stringtodate(std::string &strtim, std::string strsep = "-") {
		stringtodate(&data, strtim, strsep);
	}
	SQLSMALLINT GetYear() { return data.year; }
	void SetYear(SQLSMALLINT year) { data.year = year; }
	SQLUSMALLINT GetMonth() { return data.month; }
	void SetMonth(SQLUSMALLINT month) { data.month = month; }
	SQLUSMALLINT GetDay() { return data.day; }
	void SetDay(SQLUSMALLINT day) { data.day = day; }
	SQLUSMALLINT GetHour() { return data.hour; }
	void SetHour(SQLUSMALLINT hour) { data.hour = hour; }
	SQLUSMALLINT GetMinute() { return data.minute; }
	void SetMinute(SQLUSMALLINT minute) { data.minute = minute; }
	SQLUSMALLINT GetSecond() { return data.second; }
	void SetSecond(SQLUSMALLINT second) { data.second = second; }
	SQLUINTEGER GetFraction() { return data.fraction; }
	void SetFraction(SQLUINTEGER fraction) { data.fraction = fraction; }
	void SetDate(SQLSMALLINT year, SQLUSMALLINT month, SQLUSMALLINT day) {
		SetYear(year);
		SetMonth(month);
		SetDay(day);
	}
	operator TIMESTAMP_STRUCT() const { return data; }
	COdbcDateTime &operator=(const TIMESTAMP_STRUCT &r) {
		data = r;
		return *this;
	}

protected:
	TIMESTAMP_STRUCT data;
};

class COdbcConnection {
public:
	COdbcConnection() { Initialize(); }
	COdbcConnection(std::string Driver, std::string Server, std::string User,
					std::string Password, std::string Database) {
		Initialize();
		m_driver = Driver;
		m_server = Server;
		m_database = Database;
		m_user = User;
		m_password = Password;
	}
	virtual ~COdbcConnection() {
		if (m_Connect > -1) this->Disconnect();
		if (m_hstmt) SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		if (m_hdbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
		if (m_henv) SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
	}

protected:
	void Initialize() {
		m_driver = "";
		m_server = "";
		m_user = "";
		m_password = "";
		m_database = "";
		m_ConnectionString = "";
		m_Connect = -1;
	}

protected:
	SQLHENV m_henv = SQL_NULL_HENV;	   // Environment
	SQLHDBC m_hdbc = SQL_NULL_HDBC;	   // Connection handle
	SQLHSTMT m_hstmt = SQL_NULL_HSTMT; // Statement handle
	std::string m_driver;
	std::string m_server;
	std::string m_user;
	std::string m_password;
	std::string m_database;
	std::string m_ConnectionString;
	char m_ConnectAttr[256];
	int m_Connect;

public:
	std::string Get_Driver() { return m_driver; }
	void Set_Driver(std::string driver) {
		m_driver = driver;
		Get_ConnectionString();
	}
	std::string Get_Server() { return m_server; }
	void Set_Server(std::string server) {
		m_server = server;
		Get_ConnectionString();
	}
	std::string Get_Database() { return m_database; }
	void Set_Database(std::string database) {
		m_database = database;
		Get_ConnectionString();
	}
	std::string Get_UserID() { return m_user; }
	void Set_UserID(std::string UserID) { m_user = UserID; }
	std::string Get_Password() { return m_password; }
	void Set_Password(std::string Password) { m_password = Password; }
	std::string Get_ConnectionString() {
		m_ConnectionString = "DRIVER={" + m_driver + "};SERVER=" + m_server +
							 ";DATABASE=" + m_database + ";UID=" + m_user +
							 ";PWD=" + m_password + ";";
		return m_ConnectionString;
	}
	SQLRETURN DriverConnect() {
		SQLCHAR outstr[2048];
		SQLSMALLINT outstrlen = 2048;
		std::string connctionstring = Get_ConnectionString();
		SQLRETURN ret;
		ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
		ret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION,
							(SQLPOINTER *)SQL_OV_ODBC3, 0);
		ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
		// Set login timeout to 10 seconds
		ret = SQLSetConnectAttr(m_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)10, 0);
		ret = SQLDriverConnect(m_hdbc, NULL, (SQLCHAR *)connctionstring.c_str(),
							   SQL_NTS, outstr, sizeof(outstr), &outstrlen,
							   SQL_DRIVER_NOPROMPT);
		if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO)) {
			m_Connect = ret;
		} else {
			m_Connect = ret;
		}
		return ret;
	}
	SQLRETURN Disconnect() { return SQLDisconnect(m_hdbc); }
	SQLRETURN Get_ConnectAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr,
							  SQLINTEGER BufferLength,
							  SQLINTEGER *StringLengthPtr) {
		return SQLGetConnectAttr(m_hdbc, Attribute, ValuePtr, BufferLength,
								 StringLengthPtr);
	}
	SQLHENV Get_EnvironmentHandle() { return m_henv; }
	SQLHDBC Get_ConnectionHandle() { return m_hdbc; }
	SQLRETURN SetCommitMode(unsigned long *val) {
		if (*val == SQL_AUTOCOMMIT_OFF || *val == SQL_AUTOCOMMIT_ON) {
			return SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
									 (SQLPOINTER)val, 0);
		}
		return SQL_ERROR;
	}
	SQLRETURN SetAutoCommitModeOn() {
		return SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
								 (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
	}
	SQLRETURN SetAutoCommitModeOff() {
		return SQLSetConnectAttr(m_hdbc, SQL_ATTR_AUTOCOMMIT,
								 (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
	}
	// SQL_HANDLE_DBC
	/*
	SQLRETURN SQLGetConnectAttr(
	 SQLHDBC        ConnectionHandle,
	 SQLINTEGER     Attribute,
	 SQLPOINTER     ValuePtr,
	 SQLINTEGER     BufferLength,
	 SQLINTEGER *   StringLengthPtr);
	*/
	SQLRETURN GetConnectAttr(SQLINTEGER Attribute, SQLPOINTER ValuePtr,
							 SQLINTEGER BufferLength,
							 SQLINTEGER *StringLengthPtr) {
		return SQLGetConnectAttr(m_hdbc, Attribute, ValuePtr, BufferLength,
								 StringLengthPtr);
	}
	SQLRETURN EndTran(SQLSMALLINT CompletionType) {
		if (CompletionType == SQL_COMMIT || CompletionType == SQL_ROLLBACK) {
			return SQLEndTran(SQL_HANDLE_DBC, m_hdbc, CompletionType);
		}
		return SQL_ERROR;
	}
	SQLRETURN EndTranCommit() {
		return SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_COMMIT);
	}
	SQLRETURN EndTranRollback() {
		return SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_ROLLBACK);
	}
};

class COdbcCommand {
public:
	COdbcCommand(COdbcConnection *pcon) {
		_con = pcon;
		m_CommandString = "";
		m_henv = _con->Get_EnvironmentHandle();
		m_hdbc = _con->Get_ConnectionHandle();
		SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	}
	virtual ~COdbcCommand() {
		if (m_hstmt != SQL_NULL_HSTMT) {
			SQLFreeStmt(m_hstmt, 0);
			m_hstmt = SQL_NULL_HSTMT;
		}
	}
	SQLRETURN Set_CursorName(std::string CursorName) {
		return SQLSetCursorName(m_hstmt, (SQLCHAR *)CursorName.c_str(),
								SQL_NTS);
	}
	SQLRETURN BindCol(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber,
					  SQLSMALLINT TargetType, SQLPOINTER TargetValue,
					  SQLLEN BufferLength, SQLLEN *StrLen_or_Ind) {
		return SQLBindCol(StatementHandle, ColumnNumber, TargetType,
						  TargetValue, BufferLength, StrLen_or_Ind);
	}
	SQLRETURN mSQLBindParameter(SQLUSMALLINT ipar, SQLSMALLINT fParamType,
								SQLSMALLINT fCType, SQLSMALLINT fSqlType,
								SQLULEN cbColDef, SQLSMALLINT ibScale,
								SQLPOINTER rgbValue, SQLLEN cbValueMax,
								SQLLEN *pcbValue) {
		return SQLBindParameter(m_hstmt, ipar, fParamType, fCType, fSqlType,
								cbColDef, ibScale, rgbValue, cbValueMax,
								pcbValue);
	}

	SQLRETURN mSQLExecDirect() {
		SQLRETURN ret;
		ret =
			SQLExecDirect(m_hstmt, (SQLCHAR *)m_CommandString.c_str(), SQL_NTS);
		return ret;
	}
	SQLRETURN mSQLExecDirect(std::string command) {
		m_CommandString = command;
		return mSQLExecDirect();
	}
	SQLRETURN mSQLPrepare() {
		SQLINTEGER length = (SQLINTEGER)m_CommandString.length();
		SQLRETURN ret =
			SQLPrepare(m_hstmt, (SQLCHAR *)m_CommandString.c_str(), length);
		return ret;
	}
	SQLRETURN mFetch() { return SQLFetch(m_hstmt); }
	SQLRETURN mFetch(SQLHSTMT hstmt) {
		m_hstmt = hstmt;
		return mFetch();
	}
	SQLRETURN SQL_API BindCol(SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
							  SQLPOINTER TargetValue, SQLLEN BufferLength,
							  SQLLEN *StrLen_or_Ind) {
		return SQLBindCol(m_hstmt, ColumnNumber, TargetType, TargetValue,
						  BufferLength, StrLen_or_Ind);
	}
	SQLRETURN GetData(SQLUSMALLINT Col_or_Param_Num, SQLSMALLINT TargetType,
					  SQLPOINTER TargetValuePtr, SQLLEN BufferLength,
					  SQLLEN *StrLen_or_IndPtr) {
		return SQLGetData(m_hstmt, Col_or_Param_Num, TargetType, TargetValuePtr,
						  BufferLength, StrLen_or_IndPtr);
	}
	SQLRETURN GetData(SQLHSTMT StatementHandle, SQLUSMALLINT Col_or_Param_Num,
					  SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr,
					  SQLLEN BufferLength, SQLLEN *StrLen_or_IndPtr) {
		m_hstmt = StatementHandle;
		return this->GetData(Col_or_Param_Num, TargetType, TargetValuePtr,
							 BufferLength, StrLen_or_IndPtr);
	}
	SQLHSTMT GetStatementHandle() { return m_hstmt; }
	std::string GetCommandString() { return m_CommandString; }
	void SetCommandString(std::string CommandString) {
		m_CommandString = CommandString;
	}
	SQLRETURN BindParameterBinary(SQLUSMALLINT ipar, SQLSMALLINT fParamType,
								  SQLULEN cbColDef, SQLSMALLINT ibScale,
								  SQLPOINTER rgbValue, SQLLEN *pcbValue) {
		std::stringstream ss;
		SQLCHAR *val = (SQLCHAR *)rgbValue;
		for (SQLULEN i = 0; i < cbColDef; i++) {
			ss << std::hex << val[i];
		}
		std::string valstring = "0x" + ss.str();
		SQLLEN cbValueMax = valstring.length() + 1;
		char *valchar = new char[cbValueMax];
		memset(valchar, 0, cbValueMax);
		sprintf(valchar, "%s", valstring.c_str());
		SQLRETURN ret =
			mSQLBindParameter(ipar, fParamType, SQL_C_CHAR, SQL_CHAR, cbColDef,
							  ibScale, rgbValue, cbValueMax, pcbValue);
		delete[] valchar;
		return ret;
	}
	SQLRETURN BindParameterDateTime(SQLUSMALLINT ipar, SQLSMALLINT fParamType,
									SQLPOINTER rgbValue, SQLLEN *pcbValue) {
		SQLRETURN ret;
		SQL_TIMESTAMP_STRUCT *date = (SQL_TIMESTAMP_STRUCT *)rgbValue;
		std::string datestring = COdbcDateTime::to_string(date, '-', true);
		SQLLEN cbValueMax = datestring.length() + 1;
		char *datechar = new char[cbValueMax];
		memset(datechar, 0, cbValueMax);
		sprintf(datechar, "%s", datestring.c_str());
		ret = this->mSQLBindParameter(ipar, SQL_PARAM_INPUT, SQL_C_CHAR,
									  SQL_CHAR, cbValueMax, 0, datechar,
									  cbValueMax, pcbValue);
		delete[] datechar;
		return ret;
	}

protected:
	// member
	std::string m_CommandString;

protected:
	SQLHENV m_henv = SQL_NULL_HENV;	   // Environment
	SQLHDBC m_hdbc = SQL_NULL_HDBC;	   // Connection handle
	SQLHSTMT m_hstmt = SQL_NULL_HSTMT; // Statement handle
private:
	COdbcConnection *_con;
};
class COdbcColumn {
public:
	COdbcColumn() { mType = eSqlType::_unknown; }
	COdbcColumn(std::string table_catalog, std::string table_schema,
				std::string table_name, std::string column_name,
				std::string ordinal_position, std::string column_default,
				std::string is_nullable, std::string data_type,
				std::string character_maximum_length,
				std::string character_octet_length,
				std::string numeric_precision, std::string numeric_scale,
				std::string datetime_precision, std::string character_set_name,
				std::string collation_name, eSqlType type = _unknown) {
		SetValue(table_catalog, table_schema, table_name, column_name,
				 ordinal_position, column_default, is_nullable, data_type,
				 character_maximum_length, character_octet_length,
				 numeric_precision, numeric_scale, datetime_precision,
				 character_set_name, collation_name, type);
	}

	~COdbcColumn() {}

public:
	void SetValue(std::string table_catalog, std::string table_schema,
				  std::string table_name, std::string column_name,
				  std::string ordinal_position, std::string column_default,
				  std::string is_nullable, std::string data_type,
				  std::string character_maximum_length,
				  std::string character_octet_length,
				  std::string numeric_precision, std::string numeric_scale,
				  std::string datetime_precision,
				  std::string character_set_name, std::string collation_name,
				  eSqlType type = _unknown) {
		this->table_catalog = table_catalog;
		this->table_schema = table_schema;
		this->table_name = table_name;
		this->column_name = column_name;
		this->ordinal_position = ordinal_position;
		this->column_default = column_default;
		this->is_nullable = is_nullable;
		this->data_type = data_type;
		this->character_maximum_length = character_maximum_length;
		this->character_octet_length = character_octet_length;
		this->numeric_precision = numeric_precision;
		this->numeric_scale = numeric_scale;
		this->datetime_precision = datetime_precision;
		this->character_set_name = character_set_name;
		this->collation_name = collation_name;
		this->mType = type;
	}
	static void SQLNumericConv(SQL_NUMERIC_STRUCT *pv, double val) {
		double dwk0 = val;
		if (dwk0 < 0) {
			dwk0 *= -1;
			pv->sign = 0;
		}
		unsigned long long l0 = (unsigned long long)dwk0; //整数部
		double dwk1 = dwk0 - (double)l0;
		SQLCHAR cnt = 0;
		if (dwk1 != 0) {
			unsigned long long l1 = 0;
			double dwk2 = dwk1;
			unsigned long long l2 = 0;
			while (dwk2 != 0) {
				l2 = std::pow(10, cnt);
				dwk2 = dwk1 * (double)l2;
				l1 = (unsigned long long)dwk2;
				dwk2 = dwk2 - (double)l1;
				if (cnt == pv->scale) break;
				cnt++;
			}
			if (cnt < pv->scale) pv->scale = cnt;
			dwk1 = dwk0 * (double)std::pow(10.0, cnt);
			union my__128 mok;
			for (int i = 0; i < 16; i++)
				mok.y[i] = 0;
			mok.x[0] = (unsigned long long)dwk1;
			mok.x[1] = 0;
			for (int i = 0; i < 16; i++)
				pv->val[i] = mok.y[i];
		}
	}

	static void SQLNumericConv2(SQL_NUMERIC_STRUCT *pv, double val) {
		pv->precision = 38;
		pv->scale = 5;
		double dwk0 = val;
		if (dwk0 < 0) {
			dwk0 *= -1;
			pv->sign = 0;
		} else {
			pv->sign = 1;
		}

		unsigned long long l0 = (unsigned long long)dwk0; //整数部
		double dwk1 = dwk0 - (double)l0;
		SQLCHAR cnt = 0;
		unsigned long long l1 = 0;
		double dwk2 = dwk1;
		unsigned long long l2 = 0;
		while (dwk2 != 0) {
			l2 = std::pow(10, cnt);
			dwk2 = dwk1 * (double)l2;
			l1 = (unsigned long long)dwk2;
			dwk2 = dwk2 - (double)l1;
			if (cnt == pv->scale) break;
			cnt++;
		}
		if (cnt < pv->scale) pv->scale = cnt;
		dwk1 = dwk0 * (double)std::pow(10.0, cnt);
		union my__128 mok;
		for (int i = 0; i < 16; i++)
			mok.y[i] = 0;
#ifdef _WIN32
		mok.x[0] = (unsigned long long)dwk1;
		mok.x[1] = 0;
#else
		mok.z = dwk1;
#endif
		for (int i = 0; i < 16; i++)
			pv->val[i] = mok.y[i];
	}

	static double StrtoHextoVal(SQL_NUMERIC_STRUCT NumStr) {
		unsigned long long myvalue, divisor;
		double final_val;
		unsigned long long value = 0;
		int i = 1, last = 1, current;
		int a = 0, b = 0;

		for (i = 0; i <= 15; i++) {
			current = (int)NumStr.val[i];
			a = current % 16; // Obtain LSD
			b = current / 16; // Obtain MSD

			value += last * a;
			last = last * 16;
			value += last * b;
			last = last * 16;
		}
		myvalue = value;
		divisor = 1;
		if (NumStr.scale > 0) {
			for (int j = 0; j < NumStr.scale; j++)
				divisor = divisor * 10;
		}
		final_val = (double)myvalue / (double)divisor;
		if (NumStr.sign != 1) final_val *= -1;
		return final_val;
	}
	static void SetNumericValue(SQL_NUMERIC_STRUCT *Num, double value) {
		memset(Num->val, 0, 16);
		double wok = value;
		for (int i = 0; i < Num->scale; i++) {
			wok = wok * 10.0;
		}
	}
	static double StrToVal(SQL_NUMERIC_STRUCT &Num) {
		double final_val = 0.0;
		unsigned long long wok = 0;
		for (int i = 0; i < SQL_MAX_NUMERIC_LEN; i++) {
			unsigned long long tyu = (unsigned long long)Num.val[i];
			if (i == 0) {
				wok += tyu;
			} else {
				unsigned long long kake = 1;
				for (int j = 1; j <= i; j++) {
					kake = kake * 256;
				}
				tyu = tyu * kake;
				wok += tyu;
			}
		}
		unsigned long long divisor = 1;
		if (Num.scale > 0) {
			for (int j = 0; j < Num.scale; j++)
				divisor = divisor * 10;
		}
		final_val = (double)wok / (double)divisor;
		if (Num.sign != 1) final_val *= -1;
		return final_val;
	}

	static std::string NumericToString(SQL_NUMERIC_STRUCT *num) {
		std::string ret = "";
		union my__128 uni;
		memcpy(uni.y, num->val, 16);
		long double wk;
#ifdef _WIN32
		wk = uni.x[0];
#else
		wk = uni.z;
#endif
		if (num->sign == 0) wk *= -1;
		for (int i = 0; i < num->scale; i++) {
			wk = wk / 10;
		}
		char *buf = new char[64];
		sprintf(buf, "%Lf", wk);
		ret = buf;
		delete[] buf;
		return ret;
	}
	static char *NumericToString(SQL_NUMERIC_STRUCT *num, char *buf) {
		union my__128 uni;
		memcpy(uni.y, num->val, 16);
		long double wk;
#ifdef _WIN32
		wk = uni.x[0];
#else
		wk = uni.z;
#endif
		if (num->sign == 0) wk *= -1;
		for (int i = 0; i < num->scale; i++) {
			wk = wk / 10;
		}
		if (buf == nullptr) buf = new char[64];
		sprintf(buf, "%Lf", wk);
		return buf;
	}

public:
	std::string table_catalog;
	std::string table_schema;
	std::string table_name;
	std::string column_name;
	std::string ordinal_position;
	std::string column_default;
	std::string is_nullable;
	std::string data_type;
	std::string character_maximum_length;
	std::string character_octet_length;
	std::string numeric_precision;
	std::string numeric_scale;
	std::string datetime_precision;
	std::string character_set_name;
	std::string collation_name;
	eSqlType mType;
};

class COdbcRecord {
public:
	COdbcRecord() {
		m_Modify = _NoModify;
		m_newF = false;
	}
	virtual ~COdbcRecord() {}
	eRecordModify get_Modify() { return m_Modify; }
	void set_Modify(eRecordModify value) { m_Modify = value; }
	bool get_Flg() { return m_newF; }

protected:
	eRecordModify m_Modify;
	void set_Flg(bool value) { m_newF = value; }

private:
	bool m_newF;
};

class COdbcTable {
public:
	COdbcTable() {
		m_Count = 0;
		m_TableName = "";
	}
	virtual ~COdbcTable() { m_Column.clear(); }

protected:
	int m_Count;
	std::string m_SqlSELECT;
	std::string m_SqlINSERT;
	std::string m_SqlUPDATE;
	std::string m_SqlDELETE;
	std::vector<COdbcColumn> m_Column;
	std::string m_TableName;

public:
	std::string Get_SELECT() { return m_SqlSELECT; }
	std::string Get_INSERT() { return m_SqlINSERT; }
	std::string Get_UPDATE() { return m_SqlUPDATE; }
	std::string Get_DELETE() { return m_SqlDELETE; }
	std::string Get_Name() { return m_TableName; }
	COdbcColumn Column(int i) { return m_Column.at(i); }
	int ColumnCount() { return m_Column.size(); }

	std::string UpdateSet(SQLPOINTER pval, COdbcColumn *col) {
		string ret = col->column_name + "=";
		std::stringstream ss;
		ss << ret;
		switch (col->mType) {
		case _unknown:
			break;
		case _bit: {
			bool *pbit = (bool *)pval;
			if (*pbit) {
				ss << 1;
			} else {
				ss << 0;
			}
		} break;
		case _tinyint:
		case _smallint: {
			SQLSMALLINT *psint = (SQLSMALLINT *)pval;
			ss << *psint;
		} break;
		case _int: {
			SQLINTEGER *pint = (SQLINTEGER *)pval;
			ss << *pint;
		} break;
		case _bigint: {
			SQLLEN *plen = (SQLLEN *)pval;
			ss << *plen;
		} break;
		case _decimal:
		case _numeric:
		case _smallmoney:
		case _money: {
			SQL_NUMERIC_STRUCT *pv = (SQL_NUMERIC_STRUCT *)pval;
			ss << COdbcColumn::StrtoHextoVal(*pv);
		} break;
		case _real:
		case _float: {
			SQLDOUBLE *pv = (SQLDOUBLE *)pval;
			ss << *pv;
		} break;
		case _date:
		case _time:
		case _datetime:
		case _datetime2:
		case _smalldatetime:
		case _datetimeoffset: {
			TIMESTAMP_STRUCT *pv = (TIMESTAMP_STRUCT *)pval;
			ss << "'" << COdbcDateTime::to_string(pv, '-', true) << "'";
		} break;
		case _char:
		case _varchar:
		case _text:
		case _nchar:
		case _nvarchar:
		case _ntext: {
			SQLCHAR *pv = (SQLCHAR *)pval;
			std::string str = (char *)pv;
			ss << "'" << str << "'";
		} break;
		case _binary: {
			size_t size = atoi(col->character_octet_length.c_str());
			SQLCHAR *pv = (SQLCHAR *)pval;
			stringstream s2;
			for (int j = 0; j < size; j++) {
				s2 << std::hex << *pv;
				pv++;
			}
			ss << "0x" << s2.str();
		} break;
		default:
			break;
		}
		ret = ss.str();
		return ret;
	}
};
} // namespace OdbcCommon
#endif // ODBCCOMMON_HPP_INCLUDED
