#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>

#include "mynumber.h"

using namespace std;

int main(int argc, char *argv[])
{
	Gtk::Main app(argc, argv);
	Gtk::Window fen;
	
	fen.set_default_size(600,600);
	GetNumber areaNumber("train-images.idx3-ubyte");

	Gtk::VBox vbox;
	Gtk::HBox hhbox;

	Gtk::Label label1("Nombre");

	Gtk::HButtonBox hbox;
		
	Gtk::Button buttonNext("Next");
	Gtk::Button buttonBefore("Before");

	hhbox.pack_start(areaNumber);
	hhbox.pack_start(label1);

	vbox.pack_start(hhbox);

	hbox.pack_start(buttonBefore);
	hbox.pack_start(buttonNext);
	vbox.pack_start(hbox);	

	fen.add(vbox);
	fen.show_all();
	
	areaNumber.nextNumber();	
	areaNumber.nextNumber();	

	areaNumber.nextNumber();	
	areaNumber.nextNumber();	
	areaNumber.nextNumber();	
	buttonNext.signal_clicked().connect(sigc::mem_fun(areaNumber,&GetNumber::nextNumber));
	buttonBefore.signal_clicked().connect(sigc::mem_fun(areaNumber,&GetNumber::beforeNumber));
	Gtk::Main::run(fen);
	return 0;
}
