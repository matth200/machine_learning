#ifndef DEFINE_NUMBER
#define DEFINE_NUMBER

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

#include <glibmm/main.h>

#include <cmath>
#include <fstream>
#include <string.h>

#include <thread>
#include <chrono>

typedef std::chrono::high_resolution_clock::time_point time_point;

class GetNumber : public Gtk::DrawingArea
{ 
public:
	GetNumber(const char* filename);
	~GetNumber();
	void nextNumber();
	void beforeNumber();
	void clear();

protected:
	bool on_loop();		

private:
	Cairo::RefPtr<Cairo::Context> cr;
	std::ifstream m_file;
	unsigned int m_index;
	unsigned char *m_data;
	time_point prevTime, actualTime;	
	bool first;
};

#endif
