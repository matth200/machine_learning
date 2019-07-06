#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>

#include <fstream>
#include <string.h>

#include "mynumber.h"

using namespace std;

void callNext(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l);
void callBefore(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l);

int main(int argc, char *argv[])
{
	Gtk::Main app(argc, argv);
	Gtk::Window fen;
	
	fen.set_default_size(600,600);
	//ouverture de la fenetre avec les images
	GetNumber areaNumber("train-images.idx3-ubyte");
		
	//ouverture du fichier avec les nombres
	ifstream f("train-labels.idx1-ubyte",ios::binary);
	int i = 0;

 	unsigned char data = 0;
	f.seekg(8+i,ios::beg);
	f.read((char*)&data,1);
	int number = int(data);
	
	Gtk::VBox vbox;
	Gtk::HBox hhbox;

	Gtk::Label label1("Nombre");
	label1.set_text(to_string(number));

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
	
	buttonNext.signal_clicked().connect(sigc::bind<GetNumber* ,int& ,ifstream& ,int& ,Gtk::Label&>(sigc::ptr_fun(&callNext),&areaNumber,i,f,number,label1));
	buttonBefore.signal_clicked().connect(sigc::bind<GetNumber* ,int& ,ifstream& ,int& ,Gtk::Label&>(sigc::ptr_fun(&callBefore),&areaNumber,i,f,number,label1));

	Gtk::Main::run(fen);
	return 0;
}

void callNext(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l)
{
	i++;
 	unsigned char data = 0;
	f.seekg(8+i,ios::beg);
	f.read((char*)&data,1);
	number = int(data);	

	n->nextNumber();
	l.set_text(to_string(number));
}
void callBefore(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l)
{
	i--;
 	unsigned char data = 0;
	f.seekg(8+i,ios::beg);
	f.read((char*)&data,1);
	number = int(data);	

	n->beforeNumber();
	l.set_text(to_string(number));
}
