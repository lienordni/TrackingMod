/*
 Author: Vatsal Teraiya
*/
#include <iostream>
#include "MostCollinear.h"
#include "base/Vector3D.h"

int main()
{

	srand(time(NULL));
	int totalPoint;
	totalPoint = (rand() / (static_cast<int>(RAND_MAX) + 1.0)) * 4 + 1;
//std::cout<<"test"<<std::endl;
	std::vector<Tracking::Vector3D <double>> pl1;
	pl1.resize(totalPoint);
	for(int i = 0 ; i < totalPoint ; i++)
	{
		pl1[i].SetX((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (10) -5);
		pl1[i].SetY((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (10) -5);
		pl1[i].SetZ(60.);
	}
	totalPoint = (rand() / (static_cast<int>(RAND_MAX) + 1.0)) * 4 + 1;
	std::vector<Tracking::Vector3D <double>> pl2;
	pl2.resize(totalPoint);
	for(int i = 0 ; i < totalPoint ; i++)
	{
		pl2[i].SetX((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (10) -5);
		pl2[i].SetY((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (10) -5);
		pl2[i].SetZ(30.);
	}
//std::cout<<"test"<<std::endl;
	totalPoint = (rand() / (static_cast<int>(RAND_MAX) + 1.0)) * 4 + 1;
	std::vector<Tracking::Vector3D <double>> pl3;
	pl3.resize(totalPoint);
//	std::cout<<"test"<<std::endl;
	for(int i = 0 ; i < totalPoint ; i++)
	{
		pl3[i].SetX((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (10) -5);
		pl3[i].SetY((rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (10) -5);
		pl3[i].SetZ(0.);
	}

//std::cout<<"test"<<std::endl;
	MostCollinear m(pl1,pl2,pl3);
	std::vector<Tracking::Vector3D <double>> temp;
	temp = m.GetBestCollinearity();
	temp[0].Print();
	temp[1].Print();
	temp[2].Print();
	std::cout<<"Best Collinearity Index: "<<m.GetBestCollinearityIndex()<<std::endl;
	
}