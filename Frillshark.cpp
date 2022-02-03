#include "MainWindow.hpp"
#include "stdafx.hpp"
Glib::RefPtr<Gtk::Builder> refBuilder;

int main(int argc, char *argv[]) {
	Gtk::Main kit(&argc, &argv);
	try {
		refBuilder = Gtk::Builder::create_from_file("MainWindow.glade");
	} catch (const Glib::FileError &ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	} catch (const Glib::MarkupError &ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	} catch (const Gtk::BuilderError &ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}
	MainWindow *win = nullptr;
	refBuilder->get_widget_derived("MainWindow", win);
	if (win) {
		kit.run(*win);
		delete win;
		return 0;
	}
	return 1;
}
