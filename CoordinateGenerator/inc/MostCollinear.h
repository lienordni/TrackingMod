/*
  Author: Vatsal Teariya
  Guidelines: Before calling function 'GetBestCollinearityIndex', calling 'GetBestCollinearity is must
*/


#include <iostream>
#include "base/Vector3D.h"

class MostCollinear
{
private:
	Tracking::Vector3D<double> bestCollinear[3];
	double bestCollinearityIndex = 1000000000.;
	std::vector<Tracking::Vector3D <double>> plane1;
	std::vector<Tracking::Vector3D <double>> plane2;
	std::vector<Tracking::Vector3D <double>> plane3;
public:
	MostCollinear(std::vector<Tracking::Vector3D <double>> p1, std::vector<Tracking::Vector3D <double>> p2, std::vector<Tracking::Vector3D <double>> p3);
	double GetCollinearityIndex(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2, Tracking::Vector3D<double> p3);
	std::vector<Tracking::Vector3D <double>> GetBestCollinearity();
	double GetBestCollinearityIndex() {return bestCollinearityIndex;}
};