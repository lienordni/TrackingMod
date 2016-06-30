#include <iostream>
#include <math.h>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>
#include "Coordinates.h"

typedef Tomography::Properties Detector;
typedef Tracking::Vector3D<double> Point;
using namespace Tomography;

Point Transform(Point p,double dx,double dy,double dt){
	double x,y,z,xx,yy,zz;
	x=p.x();
	y=p.y();
	z=p.z();
	xx=dx+(cos(dt)*x-sin(dt)*y);
	yy=dy+(sin(dt)*x+cos(dt)*y);
	zz=z;
	Point output(xx,yy,zz);
	return output;
}

int main()
{
	double pi=3.141592658979;
	Point p1(3,2,75),p2;
	p2=Transform(p1,0,0,atan(1.5));
	std::cout<<p2.x()<<", "<<p2.y()<<", "<<p2.z()<<std::endl;
}

