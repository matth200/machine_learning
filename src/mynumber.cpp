#include "mynumber.h"
using namespace std;

GetNumber::GetNumber(const char* filename)
{
	m_index = 0;
	m_data = new unsigned char[784];
	memset((char*)m_data,0,784);	

	m_file.open(filename,ios::binary);
	m_file.seekg(16,ios::beg);
	
	Glib::signal_timeout().connect(sigc::mem_fun(*this,&GetNumber::on_loop),700);
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
	Cairo::RefPtr<Cairo::Context> cr = this->get_window()->create_cairo_context();

	Gtk::Allocation allocation = get_allocation();

	const int width = allocation.get_width();
	const int height = allocation.get_height();

	//get the info about the pixels
	m_file.seekg(16+m_index*784,ios::beg);
	memset((char*)m_data,0,784);
	m_file.read((char*)m_data,784);
	
	m_index++;
	cr->set_source_rgb(1.0,1.0,1.0);
	cr->paint();

	for(int i(0);i<784;i++)
	{
		cr->set_source_rgb(m_data[i]/255.0,m_data[i]/255.0,m_data[i]/255.0);

		int a = 10+i%28, b = 10+i/28;
		
		cr->move_to(a,b);
		cr->line_to(a+30,b);
		cr->line_to(a+30,b+30);
		cr->line_to(a,b+30);
		
		cr->fill();
	}
	
	return 1;
}
