//#ifndef ThisClass
//#define ThisClass

#include <iostream>

class Coordinates{
public:
	int N;          						// Number of Detectors
	int M;
	double temp;								
	double coordinate[3][30];				//
	void CoGenerator(int N, int M);
	double GenRandom(double min, double max);
	void Print();

};
//#endif