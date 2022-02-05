#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include "stdafx.hpp"

class MainWindow : public Gtk::ApplicationWindow {
public:
	MainWindow(BaseObjectType *obj,
			   Glib::RefPtr<Gtk::Builder> const &m_builder_)
		: Gtk::ApplicationWindow(obj), m_builder{m_builder_} {
		setInit_display();
	}
	virtual ~MainWindow() = default;

protected:
	Gtk::ComboBoxText *m_odbccombo = nullptr;
	Gtk::Entry *m_servername = nullptr;
	Gtk::Entry *m_user = nullptr;
	Gtk::Entry *m_pass = nullptr;
	Gtk::Entry *m_database = nullptr;
	Gtk::Label *m_connection = nullptr;
	Gtk::Button *m_button1 = nullptr;
	virtual void on_button1_clicked();
	SQLRETURN GetDrivers(SQLCHAR *buf);

private:
	std::string m_ConnectionString;
	Glib::RefPtr<Gtk::Builder> m_builder;
	void setInit_display();
	void on_hide_window();
	bool Sqlite3Find();
	std::string FindSettings(std::string &name);
	int SelectSettings(std::string &name, std::string *value, double *dvalue);
	int InsertSettings(std::string &name, std::string &value);
	void UpdateSettings(std::string &name, std::string &value, double dvalue);
};
#endif
