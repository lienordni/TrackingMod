/* Author(s) : Indroneil Kanungo, Vishal Singh, Gargi Kekre
 *   Date : 28th June 2016
 *      Track ValidationV3
 *      Incorporating larger cluster sizes for validation.
 *      
 *       
 */     
#include <iostream>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>
#include "Coordinates.h"
typedef Tomography::Properties Detector;
typedef Tracking::Vector3D<double> LienVector;
using namespace Tomography;

LienVector GetStripCoordinate(double x, double y, double z) {
  int tmp = 0;
  LienVector temp;
  temp.SetX(-50 + (31-x) * 3.125 + 1.5625);
  temp.SetY(-50. + y * 3.125 + 1.5625);
  temp.SetZ(z);

  return temp;
}

std::vector<int> GetScintillatorNoFromCoordinate(LienVector v,double err) {
        std::vector<int> output;
        double zz=v.z();
        double xx=(zz==-105)?(v.x()):((v.x()));//
        int low,high;
        low= ((xx<0)?(int(xx/18)-1):(int(xx/18)))*18;
        high=low+18;
        output.push_back(low/18 + 4);
        if(xx-low<=err)
        {
                output.push_back(low/18 + 3);
        }
        else if(high - xx <= err)
        {
                output.push_back(low/18 + 5);
        }
        return output;
}

int main(int argc,char* argv[]) {
        std::string temp_str = std::to_string(atoi(argv[1]));
        temp_str += ".root";
        Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
        Detector *MT1 = new GlassRpc(2, "MT1", -75, 31);
        Detector *MT2 = new GlassRpc(4, "MT2", 45, 31);
        Detector *ScintTop = new TriggeringPlane(2, "ScintTop", 105, -1);
        Detector *ScintBottom = new TriggeringPlane(2, "ScintBottom", -105, 7);
        SetupManager *setup = SetupManager::instance();
        setup->Register(MT2);
	setup->Register(MT1);
        setup->Register(ScintTop);
        setup->Register(ScintBottom);
        int evNo, i, j, k, l, h;
        double x, y, z, err=2.0;
        Coordinates c;
        std::vector<LienVector> coords(2);
        LienVector TopIntersection, BottomIntersection;
        std::vector<Detector *> DetVect = setup->GetDetectorVector("GLASS");
        std::vector<Detector *> ScintVect = setup->GetDetectorVector("TRG");
	std::vector<int> FiredStripVect[DetVect.size()][2];
	std::vector<LienVector> Points[DetVect.size()];
	LienVector temp;
	
        int fst,fsb,count=0;
        std::vector<int> cfst, cfsb;
        int NonShowerEvents=0, SinglePixelEvents=0, ValidEvents=0;
	int ClusterSizeScint, ClusterSizeDet;
	bool LessThanClusterSize, EventValid;
	if(argc>=3)
	{
		ClusterSizeDet=atoi(argv[2]);
	}
	else
		ClusterSizeDet=1;
	ClusterSizeScint=1;
	if(argc == 4)
	{
		err=1.0*atoi(argv[3]);
	}
	else
		err=0.0;
	for(evNo=0;evNo<1000;++evNo)
	{
		setup->SetEventDetected("TRG",evNo);
		if(setup->EventDetected())
		{
			if(ScintVect[0]->GetPlane(0)->GetFiredStripsVector().size()<=ClusterSizeScint && ScintVect[1]->GetPlane(0)->GetFiredStripsVector().size()<=ClusterSizeScint)
			{
				NonShowerEvents++;
				Points[0].clear();
				Points[1].clear();
				setup->SetEventDetected("GLASS",evNo);
				LessThanClusterSize=true;
				for(i=0;i<DetVect.size();i++)
				{
					for(j=0;j<DetVect[i]->GetScintillatorPlaneVector().size();j++)
					{
						LessThanClusterSize&=(DetVect[i]->GetScintillatorPlaneVector()[j]->GetFiredStripsVector().size()<=ClusterSizeDet && DetVect[i]->GetScintillatorPlaneVector()[j]->GetFiredStripsVector().size()>0); //
					}
				}
				if(LessThanClusterSize)
				{
					SinglePixelEvents++;
					for(i=0;i<DetVect.size();i++)
					{
						for(j=0;j<DetVect[i]->GetScintillatorPlaneVector().size();j++)
						{
							FiredStripVect[i][j]=DetVect[i]->GetScintillatorPlaneVector()[j]->GetFiredStripsVector();
						}
					}
					for(i=0;i<DetVect.size();i++)
					{
						for(j=0;j<FiredStripVect[i][0].size();j++)
						{
							for(k=0;k<FiredStripVect[i][1].size();k++)
							{
								x=FiredStripVect[i][0][j];
								y=FiredStripVect[i][1][k];
								temp=GetStripCoordinate(x,y,DetVect[i]->GetZPos());
								Points[i].push_back(temp);
							}
						}
					}
					EventValid=false;
					for(i=0;i<Points[0].size();++i)
					{
						for(j=0;j<Points[1].size();++j)
						{
							c.SetP1(Points[0][i]);
	                	                	c.SetP2(Points[1][j]);
	        	                	        TopIntersection = c.GetPOI(ScintTop,false);
        	        	                	BottomIntersection = c.GetPOI(ScintBottom,true);
							
							fst=ScintVect[0]->GetPlane(0)->GetFiredStripsVector()[0];
                        			        fsb=ScintVect[1]->GetPlane(0)->GetFiredStripsVector()[0];
			                                cfst=GetScintillatorNoFromCoordinate(TopIntersection,err);
                        			        cfsb=GetScintillatorNoFromCoordinate(BottomIntersection,err);

							if((fsb==cfsb[0] || fsb == cfsb[1]) && (fst==cfst[0] || fst==cfst[1]))
							{
                                       				EventValid=true;
								break;
                			                }
						}
						if(EventValid)
							break;
					}
					if(EventValid)
					{
						ValidEvents++;
						std::cout<<"Event Number : "<<evNo<<std::endl;
					}
				}
			}
		}
	}
	std::cout<<"Non Shower Events : "<<NonShowerEvents<<std::endl;
	std::cout<<"Single Pixel Events: "<<SinglePixelEvents<<std::endl;
	std::cout<<"Valid Events : "<<ValidEvents<<std::endl;
	std::cout<<"Validity % : "<<ValidEvents*100.0/SinglePixelEvents<<std::endl;
}















