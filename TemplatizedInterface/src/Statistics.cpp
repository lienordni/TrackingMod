#include "Statistics.h"

Statistics::Statistics() {

}

void Statistics::Initialize() {

	detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
	FiredXStrips.resize(32);
	FiredYStrips.resize(32);
	TotalCluster.resize(1024);
for(int i = 0 ; i < TotalCluster.size() ; i++)
	TotalCluster[i] = 0;
}

void Statistics::GetXPlaneHistogram() {

//Application *app = new TApplication("Histograms", NULL, NULL);
std::vector<TCanvas*> c;
c.resize(detectors.size());
int count[] = {0,0};
for(int i = 0 ; i < c.size() ; i++)
{
	c[i] = new TCanvas(detectors[i]->GetName().c_str(), "Fired Strip/Pixel Data", 200, 10);
	c[i]->Divide(2,2);
}

  int nxbins = 32;
  int nybins = 32;
  int pbins = 1024;
  int xlow = 0;
  int xhigh = 10;
 
 	std::vector<TH1F*> histogram;
 	histogram.resize(detectors.size()*3);
for(int i = 0 ; i < histogram.size() ; i = i+3)
{
	histogram[i] = new TH1F(detectors[i/3]->GetPlane(0)->GetName().c_str(), "Data", nxbins, 0, nxbins);
	histogram[i+1] = new TH1F(detectors[i/3]->GetPlane(1)->GetName().c_str(), "Data", nybins, 0, 32);
	histogram[i+2] = new TH1F(("pixels_"+detectors[i/3]->GetPlane(0)->GetName()).c_str(), "Data", pbins,0, 1024);
}
 // TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);

//TCanvas *c1 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);

int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
  //  Tracking::Vector3D<double> temp;
   // std::vector<HittedPixel *> hittedPixelVector;

   // std::vector<Detector *> detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");

    for (int evNo = 0; evNo < numOfEvents; evNo++) {
     //std::cout<<" EvNo : "<< evNo << std::endl;

      for (int j = 0; j < detectors.size(); j++) {
        detectors[j]->SetFiredStripsVector(evNo);
        //std::cout<<"Hello Vatsal" << std::endl;
        //std::cout<<"Value : "<< detectors[j]->GetPlane(0)->GetFiredStripsVector().size() <<std::endl;

      //  detectors[j]->GetPlane(0)->GetFiredStripsVector().size()
       // for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) 
       // {
        	histogram[j*3]->Fill(detectors[j]->GetPlane(0)->GetFiredStripsVector().size());	
        
        //  for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) 
         // {
          	histogram[j*3+1]->Fill(detectors[j]->GetPlane(1)->GetFiredStripsVector().size());
         // }
           histogram[j*3+2]->Fill(detectors[j]->GetPlane(0)->GetFiredStripsVector().size() * detectors[j]->GetPlane(1)->GetFiredStripsVector().size());
          // TotalCluster[detectors[j]->GetPlane(0)->GetFiredStripsVector().size() * detectors[j]->GetPlane(1)->GetFiredStripsVector().size()]++;
      //  } 
           if(detectors[j]->GetPlane(0)->GetFiredStripsVector().size() == 0 && detectors[j]->GetPlane(1)->GetFiredStripsVector().size() == 0)
          //  if(detectors[j]->GetPlane(0)->GetFiredStripsVector().size()  || detectors[j]->GetPlane(1)->GetFiredStripsVector().size())
           	count[j]++;
          

          }
      }
      double tmp = ((double)(numOfEvents-count[0]))*100.;
       double tmp1 = ((double)(numOfEvents-count[1]))*100.;
      std::cout<<"Efficiency for Detector 1: "<<tmp/(double)numOfEvents<<"%"<<std::endl;
       std::cout<<"Efficiency for Detector 2: "<<tmp1/(double)numOfEvents<<"%"<<std::endl;
      //std::cout<<"Efficiency for Detector 1: "<<count/(double)numOfEvents * 100<<"%"<<std::endl;
        for(int i = 0 ; i < detectors.size() ; i++)
        {
        	c[i]->cd(1);
        	histogram[i*3]->Draw();

        	c[i]->cd(2);
        	histogram[i*3+1]->Draw();

        	c[i]->cd(3);
        	histogram[i*3+2]->Draw();
        }    
/*
        for(int i = 0 ; i < TotalCluster.size() ; i++)
        {
        	if(TotalCluster[i] != 0)
        	{  
        	tmp = (double)TotalCluster[i]*100;
        	std::cout<<"Total Pixels Fired: "<<i<<"   "<<"Percentage: "<<tmp/(double)numOfEvents<<std::endl;
        }
        }
*/
}

void Statistics::GetYPlaneHistogram() {

}

void Statistics::GetClusterHistogram() {

}