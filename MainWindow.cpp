#include "MainWindow.hpp"
#include "CppGen.hpp"
#include "SqliteShark.hpp"
#include "resources/odbccommon.hpp"
#include "stdafx.hpp"

extern Glib::RefPtr<Gtk::Builder> refBuilder;

void MainWindow::setInit_display() {
	// get widget
	refBuilder->get_widget("_odbccombo", m_odbccombo);
	refBuilder->get_widget("_servername", m_servername);
	refBuilder->get_widget("_user", m_user);
	refBuilder->get_widget("_pass", m_pass);
	refBuilder->get_widget("_database", m_database);
	refBuilder->get_widget("_connection", m_connection);
	refBuilder->get_widget("_button1", m_button1);

	// connect signals
	m_button1->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button1_clicked));
	this->signal_hide().connect(sigc::mem_fun(*this, &MainWindow::on_hide_window));
	Sqlite3Find();
	string value = "";
	string id("Driver");
	double dvalue = 0;
	if (SelectSettings(id, &value, &dvalue) == -1) {
		value = "";
		dvalue = -1.0;
		int ret = InsertSettings(id, value);
		UpdateSettings(id, value, dvalue);
	}
	SQLCHAR *buf = new SQLCHAR[4096];
	this->GetDrivers(buf);
	for (int i = 0; i < 16; i++) {
		char *ptr = (char *)(buf + (i * 256));
		if (*ptr == 0x0) {
			break;
		} else {
			std::string drv = ptr;
			if (value == drv) {
				dvalue = (double)i;
			}
		}
		Glib::ustring *str = new Glib::ustring((const char *)ptr);
		m_odbccombo->append(*str);
		delete str;
	}
	int n = dvalue;
	if (n > -1) m_odbccombo->set_active(n);

	id = "Server";
	if (SelectSettings(id, &value, nullptr) == -1) {
		value = "localhost";
		int ret = InsertSettings(id, value);
	}
	m_servername->set_text(value);
	id = "User";
	if (SelectSettings(id, &value, nullptr) == -1) {
		value = "";
		int ret = InsertSettings(id, value);
	}
	m_user->set_text(value);
	id = "Password";
	if (SelectSettings(id, &value, nullptr) == -1) {
		value = "";
		int ret = InsertSettings(id, value);
	}
	m_pass->set_text(value);
	id = "Database";
	if (SelectSettings(id, &value, nullptr) == -1) {
		value = "";
		int ret = InsertSettings(id, value);
	}
	m_database->set_text(value);

	delete[] buf;
}
/*************************************************
	events
*************************************************/
// close event
void MainWindow::on_hide_window() { std::cout << "hide" << std::endl; }

void MainWindow::on_button1_clicked() {
	m_ConnectionString = "Driver={" + m_odbccombo->get_active_text() + "};Server=" + m_servername->get_text() +
						 ";UID=" + m_user->get_text() + ";PWD=" + m_pass->get_text() +
						 ";Database=" + m_database->get_text() + ";";
	m_connection->set_text(m_ConnectionString);
	string filename = "./" + m_database->get_text() + ".hpp";
	Glib::ustring title("Odbc Class Header");
	Gtk::FileChooserDialog *dlg = new Gtk::FileChooserDialog(title, Gtk::FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
	dlg->add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
	dlg->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dlg->set_filename(filename);
	if (dlg->run() == Gtk::ResponseType::RESPONSE_OK) {
		filename = dlg->get_filename();
		OdbcCommon::COdbcConnection *con = new OdbcCommon::COdbcConnection();
		con->Set_Driver(m_odbccombo->get_active_text());
		con->Set_Server(m_servername->get_text());
		con->Set_UserID(m_user->get_text());
		con->Set_Password(m_pass->get_text());
		con->Set_Database(m_database->get_text());
		int re1 = con->DriverConnect();
		CppGen *cpp = new CppGen(filename, m_ConnectionString, con, false);
		int ret = cpp->Execute();
		Gtk::MessageDialog *msg = nullptr;
		if (ret == SQL_ERROR) {
			msg = new Gtk::MessageDialog(*dlg, "エラーで終了しました。登録情報を確認してください", false,
										 Gtk::MessageType::MESSAGE_ERROR, Gtk::ButtonsType::BUTTONS_OK, false);
			msg->run();
		} else if (ret == SQL_NO_DATA) {
			msg = new Gtk::MessageDialog(*dlg, "テーブルデータがありません。登録情報を確認してください", false,
										 Gtk::MessageType::MESSAGE_ERROR, Gtk::ButtonsType::BUTTONS_OK, false);
			msg->run();
		}
		if (msg) delete msg;
		delete cpp;
		delete con;
		string driver = m_odbccombo->get_active_text();
		double comid = (double)m_odbccombo->get_active_row_number();
		string server = m_servername->get_text();
		string user = m_user->get_text();
		string pass = m_pass->get_text();
		string database = m_database->get_text();
		string id = "Driver";
		UpdateSettings(id, driver, comid);
		id = "Server";
		UpdateSettings(id, server, 0);
		id = "User";
		UpdateSettings(id, user, 0);
		id = "Password";
		UpdateSettings(id, pass, 0);
		id = "Database";
		UpdateSettings(id, database, 0);
	}
	dlg->close();
	delete dlg;
}

SQLRETURN MainWindow::GetDrivers(SQLCHAR *buf) {
	SQLHENV henv = SQL_NULL_HENV;	// Environment
	SQLHDBC hdbc = SQL_NULL_HDBC;	// Connection handle
	SQLHENV m_henv = SQL_NULL_HENV; // Environment
	SQLHDBC m_hdbc = SQL_NULL_HDBC; // Connection handle
	SQLCHAR *drivers = buf;
	memset(drivers, 0, 4096);
	short RESULT_LEN = 256;
	SQLCHAR drivername[RESULT_LEN];
	SQLCHAR attributes[RESULT_LEN];
	SQLUSMALLINT fitch;
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	m_henv = henv;
	ret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER *)SQL_OV_ODBC3, 0);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &hdbc);
	m_hdbc = hdbc;
	// Set login timeout to 5 seconds
	ret = SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)10, 0);
	for (int i = 0; i < 16; i++) {
		if (i == 0)
			fitch = SQL_FETCH_FIRST;
		else
			fitch = SQL_FETCH_NEXT;
		drivers = buf + (i * 256);
		SQLSMALLINT len1 = RESULT_LEN;
		SQLSMALLINT len2 = RESULT_LEN;
		SQLSMALLINT len3 = RESULT_LEN;
		SQLSMALLINT len4 = RESULT_LEN;
		ret = SQLDrivers(m_henv, fitch, drivers, len1, &len2, attributes, len3, &len4);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			break;
		}
	}
	// memcpy(buf, drivers, 4096);
	ret = SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
	ret = SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
	return ret;
}
bool MainWindow::Sqlite3Find() {
	bool ret = false;
	Shark::SqliteConnection con(__SQLITEFILE__);
	con.Open();
	string sql("SELECT COUNT(*) FROM sqlite_master WHERE TYPE='table' AND "
			   "name='t_frillshark'");
	Shark::SqliteCommand com(&con);
	sqlite3_stmt *stmt = nullptr;
	char *errmsg = nullptr;
	const char *err = nullptr;
	int cnt = 0;
	int rc = com.Prepare(sql.c_str(), -1, &stmt, &err);
	if (rc == SQLITE_OK) {
		while (com.Step() == SQLITE_ROW) {
			cnt = com.Column_int(0);
			break;
		}
	}
	com.Reset();
	com.Finalize();
	Shark::SqliteCommand *pcm = new Shark::SqliteCommand(&con);
	if (cnt == 0) {
		sql = "CREATE TABLE \"t_frillshark\" (\"id\" TEXT, \"value\" TEXT, "
			  "\"nvalue\" NUMERIC, PRIMARY KEY(\"id\"))";
		pcm->setSql(sql);
		rc = pcm->Exec(errmsg);
	}
	con.Close();
	ret = true;
	return ret;
}
std::string MainWindow::FindSettings(std::string &name) {
	std::string ret = "";
	Shark::SqliteConnection con(__SQLITEFILE__);
	con.Open();
	string sql("select id, value, nvalue from t_frillshark where id = '");
	sql = sql + name + "';";
	Shark::SqliteCommand com(&con);
	sqlite3_stmt *stmt = nullptr;
	const char *err = nullptr;
	com.setSql(sql);
	int rc = com.Prepare(sql.c_str(), -1, &stmt, &err);
	if (rc == SQLITE_OK) {
		while (com.Step() == SQLITE_ROW) {
			ret = (char *)com.Column_text(1);
			break;
		}
	} else {
		InsertSettings(name, ret);
	}
	com.Reset();
	com.Finalize();
	con.Close();
	return ret;
}
int MainWindow::SelectSettings(std::string &name, std::string *value, double *dvalue = nullptr) {
	int rs = -1;
	Shark::SqliteConnection con(__SQLITEFILE__);
	string sql = "SELECT * FROM t_frillshark WHERE id = '" + name + "'";
	con.Open();
	Shark::SqliteCommand com(&con);
	sqlite3_stmt *stmt = nullptr;
	const char *err = nullptr;
	com.setSql(sql);
	int rc = com.Prepare(sql.c_str(), -1, &stmt, &err);
	if (rc == SQLITE_OK) {
		while (com.Step() == SQLITE_ROW) {
			rs = 1;
			*value = (char *)com.Column_text(1);
			if (dvalue != nullptr) {
				*dvalue = com.Column_double(2);
			}
			break;
		}
	}
	com.Reset();
	com.Finalize();
	con.Close();
	return rs;
}
int MainWindow::InsertSettings(std::string &name, std::string &value) {
	string sql("INSERT INTO t_frillshark (id,value) "
			   "VALUES ('");
	sql = sql + name + "','" + value + "')";
	Shark::SqliteConnection con(__SQLITEFILE__);
	con.Open();
	Shark::SqliteCommand com(&con);
	char *err = nullptr;
	com.setSql(sql);
	int ret = com.Exec(err);
	con.Close();
	return ret;
}
void MainWindow::UpdateSettings(std::string &name, std::string &value, double dvalue = 0.0) {
	int rs = -1;
	Shark::SqliteConnection con(__SQLITEFILE__);
	con.Open();
	std::ostringstream s1;
	s1 << dvalue;
	string sql("UPDATE t_frillshark SET value = '");
	sql = sql + value + "', nvalue = " + s1.str() + " WHERE id = '" + name + "';";
	Shark::SqliteCommand com(&con);
	char *err = nullptr;
	com.setSql(sql);
	rs = com.Exec(err);
	con.Close();
}
