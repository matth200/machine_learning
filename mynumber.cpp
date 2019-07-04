#include "mynumber.h"
using namespace std;

GetNumber::GetNumber(const char* filename)
{
	m_index = 0;
	m_data = new unsigned char[784];
	memset((char*)m_data,0,784);	

	m_file.open(filename,ios::binary);
	m_file.seekg(16,ios::beg);
	
	Glib::signal_idle().connect(sigc::mem_fun(*this,&GetNumber::on_loop));
	first = 1;
}

GetNumber::~GetNumber()
{
	for(int i(0);i<784;i++)
	{
		delete (m_data+i);
	}
}

void GetNumber::nextNumber()
{
	
	m_index++;
} 

void GetNumber::beforeNumber()
{
	m_index = (m_index<=0)?0:m_index-1;
}

void GetNumber::clear()
{
	m_file.seekg(0,ios::beg);
}

bool GetNumber::on_loop()
{	
	prevTime = chrono::high_resolution_clock::now();
	if(first)
	{
		cr = this->get_window()->create_cairo_context();
		first = 0;
	}
	Gtk::Allocation allocation = get_allocation();

	const int width = allocation.get_width();
	const int height = allocation.get_height();

	//get the info about the pixels
	m_file.seekg(16+m_index*784,ios::beg);
	memset((char*)m_data,0,784);
	m_file.read((char*)m_data,784);

	cr->set_source_rgb(1.0,1.0,1.0);
	cr->paint();

	for(int i(0);i<784;i++)
	{
		cr->set_source_rgb(m_data[i]/255.0,m_data[i]/255.0,m_data[i]/255.0);

		int a = 70+i%28*4, b = 70+i/28*4;
		
		cr->move_to(a,b);
		cr->line_to(a+30,b);
		cr->line_to(a+30,b+30);
		cr->line_to(a,b+30);
		
		cr->fill();
	}

	actualTime = chrono::high_resolution_clock::now();
	int delta = chrono::duration_cast<chrono::milliseconds>(actualTime-prevTime).count();
	if(delta<1000.0/30.0)
		this_thread::sleep_for(chrono::milliseconds(int(1000.0/30.0-delta)));
	return 1;
}
