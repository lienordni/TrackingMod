#include "Coordinates.h"
#include <math.h>

	//int N;
	//int M;
	
	//double t

	void Coordinates::CoGenerator(int N1, int M1)
	{
		N = N1;
		M = M1;
		/*
		coordinate = new double*[N];

		for(int i = 0 ; i < N ; i++)
			coordinate[i] = new double[3];
		*/
		for(int i = 0 ; i < N ; i++)
		{
			coordinate[i][2] = (double)((N-1-i)*M);
		}
		srand(time(NULL));
		coordinate[0][0] = GenRandom(-50, 50);
		coordinate[0][1] = GenRandom(-50, 50);
		coordinate[N-1][0] = GenRandom(-50, 50);
		coordinate[N-1][1] = GenRandom(-50, 50);

		for(int i = 0 ; i < N - 2 ; i++)
		{
			temp = ( (i+1) * (double)M - coordinate[0][2] ) / ( coordinate[0][2] - coordinate[N-1][2]);
			coordinate[N-i-2][0] = (coordinate[0][0] - coordinate[N-1][0]) * temp + coordinate[0][0];
			coordinate[N-i-2][1] = (coordinate[0][1] - coordinate[N-1][1]) * temp + coordinate[0][1];
		}

		//return coordinate;		
	}

	double Coordinates::GenRandom(double min, double max)
	{
		
		//return min + ((double)rand()/(double)(max-min));
		return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
	}

	void Coordinates::Print()
	{
		std::cout<<"Coordinates"<<std::endl;

		for(int i = 0 ; i < N ; i++)
		{
			std::cout<<coordinate[i][0]<<" "<<coordinate[i][1]<<" "<<coordinate[i][2]<<std::endl;
		}

		std::cout<<"Strips:"<<std::endl;
		

		for(int i = 0 ; i < N ; i++)
		{
			std::cout<<floor( (coordinate[i][0]+(double)50) /3.125 )<<" "<<floor( (coordinate[i][1]+(double)50)/3.125)<<" "<<coordinate[i][2]<<std::endl;
		}		
	}
