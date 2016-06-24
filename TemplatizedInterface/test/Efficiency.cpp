/*
 *  Authors : Indroneil Kanungo, Vishal Singh, Gargi Kekre
 *  It calculates the efficiency of the setup whilst ignoring the shower events.
 */


#include <iostream>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>
#include "base/Global.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]){
	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";
	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	SetupManager *setup = SetupManager::instance();
	Detector *MT1=new GlassRpc(2,"MT1",-75,31);
	Detector *MT2=new GlassRpc(4,"MT2",45,31);
	Detector *TopScint=new TriggeringPlane(2,"TopScint",105,-1);
	Detector *BottomScint=new TriggeringPlane(2,"BottomScint",-105,7);
	int k;	
	int topcount=0, bottomcount=0;
	int numerator=0, denominator=0;
	bool shower,rpcFired;
	int NumberOfStripsFired;
	setup->Register(MT1);
	setup->Register(MT2);
	setup->Register(TopScint);
	setup->Register(BottomScint);
	std::vector<Detector*> detVect=setup->GetDetectorVector("GLASS");
	int showercount=0;
	int ClusterSize[4];
	std::vector<Detector*> trgVect=setup->GetDetectorVector("TRG");
	for(int evNo=0;evNo<1000;++evNo) {
//		std::cout<<"======================= Event no. : "<<evNo<<"===================="<<std::endl;
		setup->SetEventDetected("TRG",evNo);
		shower=false;
		for(int i=0;i<trgVect.size();++i) {
			for(int j=0;j<trgVect[i]->GetNumOfPlanes();++j) {
				NumberOfStripsFired=trgVect[i]->GetPlane(j)->GetFiredStripsVector().size();
				shower|=(NumberOfStripsFired-1);
			}
		}

		if(!shower) {
			setup->SetEventDetected("GLASS",evNo);
/*			if(setup->EventDetected()) {
				numerator++;
			}
*/
			rpcFired=true;	
			for(int i=0;i<detVect.size();++i) {
				std::vector<ScintillatorPlane*> PlaneVect=detVect[i]->GetScintillatorPlaneVector();
				for(int j=0;j<PlaneVect.size();++j) {
					std::cout<<evNo<<"  "<<i<<"  "<<j<<"  "<<PlaneVect[j]->GetFiredStripsVector().size()<<std::endl;
					rpcFired&=(bool) (PlaneVect[j]->GetFiredStripsVector().size());
				}
				if(!rpcFired)
					break;
			}
			if(rpcFired){

				numerator++;
				std::cout<<"Numerator Incremented"<<std::endl;
				if(!setup->EventDetected()) {
					std::cout<<"setup->EventDetected = False"<<std::endl;
				}

			}
			denominator++;
		}
		std::cout<<std::endl;

	}
		
//	std::cout<<showercount<<std::endl;		
	std::cout<<numerator<<std::endl;
	std::cout<<denominator<<std::endl;
	std::cout<<"Efficiency : "<<(float) 100*numerator/denominator<<std::endl;

}
/*        Scintillator::SetWindowStartEnd(90,140);
   	Tracking::Tree *t = Tracking::Tree::instance()->GetTree();
	int numOfEvents = t->GetNumOfEvents();
        for(int evNo=0 ; evNo < numOfEvents  ; evNo++){
	trgVect[1]->GetPlane(0)->SetFiredStripsVector<false>(evNo);
	if(!trgVect[1]->GetPlane(0)->GetFiredStripsVector().size())
	   std::cout<<"---- NOT FIRED ---- " << std::endl;
	
	std::vector<Scintillator*> scintVect = trgVect[1]->GetPlane(0)->GetScintVector();
	bool fired=false;
        for(int i = 0 ; i< scintVect.size() ; i++){
	   
           Tracking::Channel *ch = 0;
	   ch = t->GetEntry(scintVect[i]->GetName(),evNo);
	//std::cout<<ch;
	   fired |= ch->size();
	   if(ch->size()){
           if(ch->at(0) < 19450 || ch->at(0) > 20550 )
		std::cout<<"Value : "<< ch->at(0) << std::endl;
                  }
		}
	 if(!fired)
		std::cout<<"--- NOT Fired ---- "<< std::endl;
	
	}
*/
	
