#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>

#include <fstream>
#include <string.h>

#include "mynumber.h"

#include "m_learning.h"

using namespace std;

void callNext(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l, Gtk::Label &l2, MachineLearning &m);
void callBefore(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l, Gtk::Label &l2, MachineLearning &m);

int main(int argc, char *argv[])
{
	Gtk::Main app(argc, argv);
	Gtk::Window fen;
	
	fen.set_default_size(600,600);

	MachineLearning machine(784);
	machine.addColumn(30);
	machine.addColumn(20);
	machine.addColumn(10);

	machine.backupTraining("save97.ml");		
	
	//ouverture de la fenetre avec les images
	GetNumber areaNumber("train-images.idx3-ubyte");
	
	machine.setInput(areaNumber.getData());
	machine.calcul();
			
	//ouverture du fichier avec les nombres
	ifstream f("train-labels.idx1-ubyte",ios::binary);
	int i = 0;

 	unsigned char data = 0;
	f.seekg(8+i,ios::beg);
	f.read((char*)&data,1);
	int number = int(data);
	
	Gtk::VBox vbox;
	Gtk::HBox hhbox;

	Gtk::Label label2("Nombre prédit --> "+to_string(machine.getPrediction())), label1(string("Nombre réel --> ")+to_string(number));

	Gtk::HButtonBox hbox;
	Gtk::VButtonBox boxResultat;	

	Gtk::Button buttonNext("Next");
	Gtk::Button buttonBefore("Before");

	hhbox.pack_start(areaNumber);

	boxResultat.pack_start(label1);
	boxResultat.pack_start(label2);
	
	hhbox.pack_start(boxResultat);

	vbox.pack_start(hhbox);

	hbox.pack_start(buttonBefore);
	hbox.pack_start(buttonNext);
	vbox.pack_start(hbox);	

	fen.add(vbox);
	fen.show_all();
	
	buttonNext.signal_clicked().connect(sigc::bind<GetNumber* ,int& ,ifstream& ,int& ,Gtk::Label&, Gtk::Label&, MachineLearning&>(sigc::ptr_fun(&callNext),&areaNumber,i,f,number,label1,label2,machine));
	buttonBefore.signal_clicked().connect(sigc::bind<GetNumber* ,int& ,ifstream& ,int& ,Gtk::Label&, Gtk::Label&, MachineLearning&>(sigc::ptr_fun(&callBefore),&areaNumber,i,f,number,label1,label2,machine));

	Gtk::Main::run(fen);
	return 0;
}

void callNext(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l, Gtk::Label &l2, MachineLearning &m)
{
	i++;
 	unsigned char data = 0;
	f.seekg(8+i,ios::beg);
	f.read((char*)&data,1);
	number = int(data);	

	n->nextNumber();
	l.set_text(string("Nombre réel --> ")+to_string(number));
	
	//prédiction du deep learning	
	m.setInput(n->getData());
	m.calcul();
	
	string texte = string("Nombre prédit --> ")+to_string(m.getPrediction())+" -  "+((number!=m.getPrediction())?to_string(m.getOutput(m.getPrediction())):string(""));	
	if(number!=m.getPrediction())
	{
		texte+="\n";
		for(int i(0);i<10;i++)
		{
			texte+=to_string(i);
			texte+=": ";
			texte+=to_string(m.getOutput(i));
			texte+="\n";
		}
	}
	l2.set_text(texte);	
}

void callBefore(GetNumber *n, int &i, ifstream &f, int &number, Gtk::Label &l, Gtk::Label &l2, MachineLearning &m)
{
	if(i>0)
	{
		i--;
 		unsigned char data = 0;
		f.seekg(8+i,ios::beg);
		f.read((char*)&data,1);
		number = int(data);	
	
		n->beforeNumber();
		l.set_text(string("Nombre réel --> ")+to_string(number));

		
		m.setInput(n->getData());
		m.calcul();
		
		string texte = string("Nombre prédit --> ")+to_string(m.getPrediction())+" -  "+((number!=m.getPrediction())?to_string(m.getOutput(m.getPrediction())):string(""));	
		if(number!=m.getPrediction())
		{
			texte+="\n";
			for(int i(0);i<10;i++)
			{
				texte+=to_string(i);
				texte+=": ";
				texte+=to_string(m.getOutput(i));
				texte+="\n";
			}
		}
		l2.set_text(texte);	
	}
}
