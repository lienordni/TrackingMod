#include <iostream>
#include <SetupManager.h>
#include <Properties.h>
#include <GlassRpc.h>
#include <ScintillatorPlane.h>

typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char* argv[]) {
	        std::string temp_str = std::to_string(atoi(argv[1]));
        temp_str += ".root";
        Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

//	Detector *MT1 = new GlassRpc(2,"MT-1",-75,31);
//	Detector *MT2 = new GlassRpc(4,"MT-2",45,31);
	Detector *ScintTop = new TriggeringPlane(2,"ScintillatorTop",105,-1);
        Detector *ScintBot = new TriggeringPlane(2,"ScintillatorBottom",-105,7); 
 
	SetupManager *setup = SetupManager::instance();
//	setup->Register(MT1);
//	setup->Register(MT2);
	setup->Register(ScintTop);
        setup->Register(ScintBot);
//        std::vector<Detector*> detVect = setup->GetDetectorVector("GLASS");
	std::vector<Detector*> scintVect= setup->GetDetectorVector("TRG");

	ScintillatorPlane::SetClusterSize(1);
	for(int evNo= 0 ;evNo <1;  evNo++){
		std::cout<<"--------------------Event No :  " << evNo << " ---------------------------------"<<std::endl;
 //       setup->SetEventDetected("GLASS",evNo);
		setup->SetEventDetected("TRG",evNo);
		std::cout<<"lienordni"<<std::endl;
	        std::cout<<"Event Detected : "<< setup->EventDetected() << std::endl;
	
	        for(int j = 0 ; j < scintVect.size() ; j++){
			for(int i = 0 ; i < scintVect[j]->GetNumOfPlanes() ; i++){
		                std::cout<< "Plane num : "<< i <<" : "<< scintVect[j]->GetPlane(i)->GetFiredStripsVector().size() << std::endl;
		                std::vector<Scintillator*> scintVector = scintVect[j]->GetPlane(i)->GetScintVector();
		                std::vector<int> stripVect = scintVect[j]->GetPlane(i)->GetFiredStripsVector();
		                int stripVectorSize = stripVect.size();
		                for(int k = 0 ; k < stripVectorSize ; k++){
		                        std::cout<<"strip no : "<< stripVect[k] << " : Value : " << scintVector[stripVect[k]]->GetValue() << std::endl;
		                }
		
	                }

	        }
        }

}




