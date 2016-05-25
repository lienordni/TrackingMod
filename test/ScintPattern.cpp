#include "Scintillator.h"
#include <iostream>
#include <TApplication.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <vector>
#include <TFile.h>
using namespace Tracking;

void GenerateTimingHistogram();

int main(){

//  RPC rpc(2,96,"FirstRpc");
//  rpc.PrintStripNames();
//  RPC rpc2(3,64,"FirstRpc");
//  rpc2.PrintStripNames();

  GenerateTimingHistogram();

}

void GenerateTimingHistogram(){
  TApplication *fApp = new TApplication("Test", NULL, NULL); 
  //Scintillator::SetStartingId(-1);
  ScintillatorPlane topPlane(2,8,-105.,false,"TopPlane");
  ScintillatorPlane bottomPlane(2,8,105.,false,"BottomPlane");
  topPlane.PrintStrips();
  std::cout<<"-----------------------------------" << std::endl;
  bottomPlane.PrintStrips();
  int nxbins = 10;
  int xlow = 0;
  int xhigh = 10;
  int nybins = 10;
  int ylow = 0;
  int yhigh = 10;
  Channel *trig = 0;
  Channel *ch1 = 0;
  Channel *ch2 = 0;

  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  c2->Divide(1, 2);
  c2->cd(1);
  Tree t("6147.root", "BSC_DATA_TREE");
  int numOfEvents = t.GetNumOfEvents();
  std::cout<<"Num of Events : "<< numOfEvents << std::endl;
  //int numOfEvents = 10000;
  // TH1F *hTrig = new TH1F("hTrig","TEST",100,20000,21000);
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);
  h2d->GetXaxis()->SetTitle("Top Scintillator Plane");
  h2d->GetYaxis()->SetTitle("Bottom Scintillator Plane");
  h2d->GetZaxis()->SetTitle("Counts");
  std::vector<Tracking::Scintillator*> scintPlane1 = topPlane.GetScintillatorPlane();
  std::vector<Tracking::Scintillator*> scintPlane2 = bottomPlane.GetScintillatorPlane();

  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    bool isShowerEvent = false;
    isShowerEvent |= topPlane.IsShowerEvent<false>(t, evNo);
    isShowerEvent |= bottomPlane.IsShowerEvent<false>(t, evNo);
    if (isShowerEvent)
      continue;
    else {
      for (int i = 0; i < scintPlane1.size(); i++) {
      //for (int i = 3; i < 6; i++) {
        ch1=0;
        //ch1 = t.GetEntry(scintPlane1[i]->GetName(), evNo);
        //if (ch1->size())
          {
          for (int j = 0; j < scintPlane2.size(); j++) {
          //for (int j = 3; j < 6; j++) {
            ch2=0;
            //ch2 = t.GetEntry(scintPlane2[j]->GetName(), evNo);
            //if (ch2->size())
            {
             // h2d->Fill(i + 1, j + 1);
             }
          }
        }
      }
    }
  }

  //h2d->Draw();
  h2d->Draw("LEGO2");
  c2->cd(2);
  h2d->Draw();
  fApp->Run();
  h2d->Print();
  // TFile f("hTrig.root","recreate"); //Open file, then write histo to it.
  TFile::Open("hTrig.root", "RECREATE");
  h2d->Write();
  c2->Modified();
  c2->Update();
}
