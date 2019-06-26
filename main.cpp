#include <iostream>
#include <fstream>

#include "m_learning.h"

using namespace std;
int main(int argc, char *argv[])
{
	cout << "lecture" << endl;	
	ifstream file("train-labels.idx1-ubyte",ios::binary);
	unsigned char number;
	/*
	for(int i(0);i<15;i++){
		file.seekg(8+i,ios::beg);
		file.read((char*)&number,1);
		cout << int(number) << endl;
	}*/	

	return 0;
}
