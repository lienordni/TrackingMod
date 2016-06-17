/*
 Author: Vatsal Teraiya
*/


#include "GlassRpc.h"
#include "CmsRpc.h"
#include "TriggeringPlane.h"
#include <vector>
#include <iostream>
#include "VisualizationHelper.h"
#include "SetupManager.h"
#include <TApplication.h>
#include "Update.h"
#include "Tree.h"
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TLegend.h>
#include "Properties.h"
 typedef Tomography::Properties Detector;
using namespace Tomography;
int main(int argc, char *argv[])
{
	TApplication *app = new TApplication("EfficiencyGraphs", NULL, NULL);
	Detector *rpc1 = new GlassRpc(2, "FirstGlassRpc", 30, 31);
	Detector *rpc2 = new GlassRpc(4, "SecondGlassRpc", 0, 31);
	int size = 20;
	int x[20] = {8000, 8200, 8400, 8600, 8800, 9000,  9100,  9200,  9300,  9400,
               9500, 9600, 9700, 9800, 9900, 10000, 10100, 10200, 10300, 10400};
	bool verbose = false;
	Tomography::SetupManager::instance()->Register(rpc1);
  	Tomography::SetupManager::instance()->Register(rpc2);
	
	std::vector<TCanvas*> c;
	std::vector<std::vector<TMultiGraph*>> mg;
	std::vector<std::vector<TGraph*>> gr;
	std::vector<Detector*> detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");

	//c.resize(detectors.size());
	mg.resize(c.size()*3);
	gr.resize(mg.size()*3);

	for(int i = 0 ; i < detectors.size() ; i++)
	{
		//c[i] = new TCanvas((detectors[i]->GetName()+" Efficiency Analysis").c_str(), " Efficiency Analysis", 800, 600);
		c.push_back(new TCanvas((detectors[i]->GetName()+" Efficiency Analysis").c_str(), " Efficiency Analysis", 800, 600));
		for(int j = 0 ; j < 3 ; j++)
		{
			mg[i][j] = new TMultiGraph();

			if(j == 0)
            mg[i][j]->SetTitle("Both X and Y ON");
        	if(j == 1)
            mg[i][j]->SetTitle("Only Y plane (ie. TOP Gap) ON");
            if(j == 2)
            mg[i][j]->SetTitle("Only X plane (ie. BOTTOM Gap) ON");

            for(int k = 0 ; k < 3 ; k++)
            {
            	gr[i][j*3+k] = new TGraph();
            	if(k == 0)
				{
					gr[i][j*3+k]->SetTitle("w.r.t both");
					gr[i][j*3+k]->SetLineColor(kRed);
				}
				if(k == 1)
				{
					gr[i][j*3+k]->SetTitle("w.r.t TOP");
					gr[i][j*3+k]->SetLineColor(kGreen);
				}
				if(k == 2)
				{
					gr[i][j*3+k]->SetTitle("w.r.t BOTTOM");
					gr[i][j*3+k]->SetLineColor(kBlue);
				}

				gr[i][j*3+k]->SetMarkerStyle(21);
				gr[i][j*3+k]->SetLineWidth(2);
				gr[i][j*3+k]->GetXaxis()->SetRangeUser(8000, 11000);


            }
		}
		
	}

  std::vector<std::string> bothOn;
  int temp1 = atoi(argv[1]); // 6794;
  std::vector<std::string> topOn;
  int temp2 = temp1 + 20;
  std::vector<std::string> bottomOn;
  int temp3 = temp2 + 20;

	std::string temp_str;
	 std::vector<std::vector<std::string>> filetype;
  for (int i = 0; i < size; i++) {
    temp_str = std::to_string(temp1);
    temp_str += ".root";
    bothOn.push_back(temp_str);
    temp1++;

    temp_str = std::to_string(temp2);
    temp_str += ".root";
    topOn.push_back(temp_str);
    temp2++;

    temp_str = std::to_string(temp3);
    temp_str += ".root";
    bottomOn.push_back(temp_str);
    temp3++;
  }
  filetype.push_back(bothOn);
  filetype.push_back(topOn);
  filetype.push_back(bottomOn);

Scintillator::SetWindowStartEnd(19450, 22500);
for(int det = 0 ; det < detectors.size() ; det++)
{ 
	std::cout<<"Detector: "<<detectors[det]->GetName()<<std::endl;
	std::cout << "////////////////////" << std::endl;
    std::cout << "////////////////////" << std::endl;
  for (int i = 0; i < 3; i++) 
  {
    std::cout << "////////////////////" << std::endl;
    std::cout << "////////////////////" << std::endl;
    for (int j = 0; j < 20; j++) 
    {

      std::cout << "===========================================================================" << std::endl;
      Tracking::Tree::instance()->ReadTree(filetype[i][j].c_str(), "BSC_DATA_TREE", 0);
      std::cout << filetype[i][j] << std::endl;
      std::cout << "===========================================================================" << std::endl;
      detectors[det]->SetEfficiency();
      detectors[det]->GetPlane(0)->SetEfficiency();
      detectors[det]->GetPlane(1)->SetEfficiency();
      if (verbose) {
        std::cout << "Eff of Det-1 : " << detectors[det]->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 1 : " << detectors[det]->GetPlane(0)->GetEfficiency() << std::endl;
        std::cout << "Eff w.r.t Plane 2 : " << detectors[det]->GetPlane(1)->GetEfficiency() << std::endl;
        std::cout << "--------------------------------------" << std::endl;
      }
     for(int k = 0 ; k < 3 ; k++)
      { 
        // both_on->SetPoint(temp1,j+1,rpc2->GetEfficiency());
        gr[det][i*3+k]->SetPoint(temp1, x[j], detectors[det]->GetEfficiency());
        gr[det][i*3+k]->SetPoint(temp2, x[j], detectors[det]->GetPlane(0)->GetEfficiency());
        gr[det][i*3+k]->SetPoint(temp3, x[j], detectors[det]->GetPlane(1)->GetEfficiency());
        temp1++;
        temp2++;
        temp3++;
      }
      
    }
  }
 }

for(int i = 0 ; i < detectors.size() ; i++)
{
  for(int j = 0 ; j < 3 ; j++)
  {
  	for(int k = 0 ; k < 3; k++)
  	{
  		mg[i][j]->Add(gr[i][j*3+k]);
  		
  		
  	}
  	    c[i]->cd(j+1);
 		mg[i][j]->Draw("ACP");
  		mg[i][j]->GetXaxis()->SetLimits(8000., 11000.);
  }	
  
}
 
  app->Run(); 
	return 0;
} 
