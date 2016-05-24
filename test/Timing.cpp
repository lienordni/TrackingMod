#include "RPC.h"
#include <iostream>
#include <TApplication.h>
#include "Scintillator.h"
#include <vector>
using namespace Tracking;

void GenerateTimingHistogram();

int main(){

/*
  RPC rpc(2,96,"FirstRpc");
  rpc.PrintStrips();
  RPC rpc2(3,64,"FirstRpc");
  rpc2.PrintStrips();
*/


  GenerateTimingHistogram();

}

void GenerateTimingHistogram(){
  TApplication *fApp = new TApplication("Test", NULL, NULL);
//  Scintillator::SetStartingId(-1);
//  ScintillatorPlane topPlane(2, 8, "Top-Plane");
//  ScintillatorPlane bottomPlane(2, 8, "Bottom-Plane");
  Tracking::ScintillatorPlane topPlane(2, 8, -105., false, "TopPlane");
  Tracking::ScintillatorPlane bottomPlane(2, 8, 105., false, "BottomPlane");
  int nxbins = 1000;
  int xlow = 19000;
  int xhigh = 21000;
  int nybins = 150;
  int ylow = -10;
  int yhigh = 140;
  Channel *trig = 0;
  Channel *ch = 0;

  TCanvas *c2 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);
  c2->Divide(1, 1);
  c2->cd(1);
  Tree t("6701.root", "BSC_DATA_TREE");
  //int numOfEvents = t.GetNumOfEvents();
  int numOfEvents = 10000;

  // TH1F *hTrig = new TH1F("hTrig","TEST",100,20000,21000);
  TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);
  std::vector<Tracking::Scintillator*> scintPlane = topPlane.GetScintillatorPlane();

  for (int evNo = 0; evNo < numOfEvents; evNo++) {
    trig = t.GetEntry("Module2_LE_CH31", evNo);
    //h2d->Fill(trig->at(0), 31);
    //for (int i = 0; i < fScintillatorPlane.size(); i++) {
    for (int i = 0; i < scintPlane.size(); i++) {
      ch = t.GetEntry(scintPlane[i]->GetName(), evNo);
      if (ch->size()) {
        //h2d->Fill(ch->at(0), scintPlane[i]->GetChannelId());
        // for (int j = 0; j < ch->size(); j++) {
        //   // h2d->Fill(ch->at(j), scintPlane[i]->GetChannelId());
        // }
      }
    }
  }

    scintPlane = bottomPlane.GetScintillatorPlane();
    for (int evNo = 0; evNo < numOfEvents; evNo++) {
    for (int i = 0; i < scintPlane.size(); i++) {
      ch = t.GetEntry(scintPlane[i]->GetName(), evNo);
      if (ch->size()) {
        h2d->Fill(ch->at(0), scintPlane[i]->GetChannelId()+10);
        // for (int j = 0; j < ch->size(); j++) {
        //   // h2d->Fill(ch->at(j), scintPlane[i]->GetChannelId());
        // }
      }
    }

  }

  scintPlane.clear();
/*
  //Now Creating Real RPC
  RPC rpc(2,96,"FirstRpc");
  scintPlane = rpc.GetRpc()->GetScintillatorPlane();
  for (int evNo = 0; evNo < numOfEvents; evNo++) {
      for (int i = 0; i < scintPlane.size(); i++) {
       // std::cout<<"Name : "<<scintPlane[i]->GetName()<<std::endl;
        ch = t.GetEntry(scintPlane[i]->GetName(), evNo);
        if (ch->size()) {
          //std::cout<<"-- Data Found --"<<std::endl;
          h2d->Fill(ch->at(0), scintPlane[i]->GetChannelId());

        }
      }

    }

*/


  h2d->Draw();
  //h2d->Print();
  // TFile f("hTrig.root","recreate"); //Open file, then write histo to it.
  //TFile::Open("hTrig.root", "RECREATE");
  //h2d->Write();
  c2->Modified();
  c2->Update();
  fApp->Run();

}