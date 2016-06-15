#include "MostCollinear.h"
#include <math.h>
MostCollinear::MostCollinear(std::vector<Tracking::Vector3D <double>> p1, std::vector<Tracking::Vector3D <double>> p2, std::vector<Tracking::Vector3D <double>> p3)
{
	plane1 = p1;
	plane2 = p2;
	plane3 = p3;
}

double MostCollinear::GetCollinearityIndex(Tracking::Vector3D<double> p1, Tracking::Vector3D<double> p2, Tracking::Vector3D<double> p3)
{
	Tracking::Vector3D<double> q1 = p1 - p2;
	Tracking::Vector3D<double> q2 = p1 - p3;

	Tracking::Vector3D<double> crossProduct;
	q1.Set(q1.x()/q1.Mag(), q1.y()/q1.Mag(), q1.z()/q1.Mag());
	q2.Set(q2.x()/q2.Mag(), q2.y()/q2.Mag(), q2.z()/q2.Mag());

	crossProduct.SetX(q1.y()*q2.z() - q2.y()*q1.z());
	crossProduct.SetY(q1.z()*q2.x() - q2.z()*q1.x());
	crossProduct.SetZ(q1.x()*q2.y() - q2.x()*q1.y());
	
	return crossProduct.Mag();
}

std::vector<Tracking::Vector3D <double>> MostCollinear::GetBestCollinearity()
{
	double tmpIndex;
	std::vector<Tracking::Vector3D <double>> temp;
	for(int i = 0 ; i < plane1.size() ; i++)
	{
		for(int j = 0 ; j < plane3.size() ; j++)
		{
			for(int k = 0 ; k < plane2.size() ; k++)
			{
				tmpIndex = GetCollinearityIndex(plane1[i], plane2[k], plane3[j]);
				if(abs(tmpIndex) < abs(bestCollinearityIndex))
				{
					bestCollinear[0] = plane1[i];
					bestCollinear[1] = plane2[k];
					bestCollinear[2] = plane3[j];

					bestCollinearityIndex = tmpIndex;
				}
			}
		}
	}
    temp.push_back(bestCollinear[0]);
    temp.push_back(bestCollinear[1]);
    temp.push_back(bestCollinear[2]);

	return temp;

}

