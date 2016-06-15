/*
 Author: Vatsal Teraiya
 Guidelines: Initialize function is first called in CoGenerator. 
 			 These are the functions which must be called in this sequence:   CoGenerator() - SetStrips() - SetStripCoordinates();
 			 Whenever making two instances: value from GetspecificCoordinate(zpos) of first instance must be passed to below layer instance-constructor
*/
#ifndef COORDINATEGENERATOR_INC_COORDINATES_H
#define COORDINATEGENERATOR_INC_COORDINATES_H

#include <iostream>
#include <vector>
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
#include "ScintillatorPlane.h"
#include "MostCollinear.h"

typedef Tomography::Properties Detector;
using namespace Tracking;
//using std::vector;
namespace Tomography{
class Coordinates{
private:
	int N;          						// Number of Detectors
	int totalDetector;
	int M;
	int signal = 0;
	double temp;		
	
	
	std::vector<double> totalStrip;
	Vector3D<double> InitialPoint;
	std::vector<Detector*> detectors;
	std::vector<Tracking::Vector3D <double>> coordinate;
	std::vector<Tracking::Vector3D <double>> stripcoord;
	std::vector<Tracking::Vector3D <double>> strip;
	std::vector<Detector*> det;
	

public:
	//Coordinates();     ooooooooooooooooooooooooooooo
	Coordinates(std::vector<Detector*> det);
	//Coordinates(Vector3D<double> init);   ooooooooooooooooooooo
	Coordinates(std::vector<Detector*> det, Vector3D<double> init);
	//~Coordinates(Vector3D<double> init);
	void InitializeVectors();

	//void CoGenerator(std::vector <Detector*> det,int N1=0);
	void CoGenerator();
	Vector3D<double> GetCoordinate(int detector);     //Enter value between 0 < detector < total detectors
	Vector3D<double> GetStripCoordinate(double x, double y, double z);
	void SetStrips(double x, double y, double z);
	void SetStripCoordinates();
	Tracking::Vector3D<double> GetStripCoordinate(int detector);
	void SetStrips();
	Tracking::Vector3D<double> GetStrip(int detector);
	double GenRandom(double min, double max);
	Vector3D<double> GetInitialPoint();
	Vector3D<double> GetSpecificCoordinate(double zpos);
	int GetLength(){return det.size();}
	void Print();

};

} //end of Tomography namespace
#endif
