/*
 * Scintillator.h
 *
 *  Created on: June 02, 2016
 *      Author: rsehgal
 */

#ifndef INC_UPDATE_H_
#define INC_UPDATE_H_

#include "Coordinates.h"
#include <TGeoMatrix.h>
#include "base/Global.h"
#include "HittedPixel.h"
#include "TGeoBBox.h"
#include "visualizer/Eve/Singleton.h"
#include "VisualizationHelper.h"
#include "TThread.h"
#include "LinesAngle.h"
#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Tree.h"
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
 #include "visualizer/Eve/Singleton.h"
 typedef Tomography::Properties Detector;

namespace Tomography {

class Update {

  TEveStraightLineSet *ls;
  int fDelay;

public:
  Update() {fDelay = 2;}
  void SetDelay(int delay){fDelay = delay;}

  void *handleRoot(void *ptr) {
    TGeoHMatrix m;
    LinesAngle l;
    Double_t trans[3] = {0., 0., 0.};
    m.SetTranslation(trans);

    int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
    Tracking::Vector3D<double> temp;
    std::vector<HittedPixel *> hittedPixelVector;

    std::vector<Detector *> detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");

    for (int evNo = 0; evNo < numOfEvents; evNo++) {
      std::cout << "======================================================" << std::endl;

      for (int j = 0; j < detectors.size(); j++) {
        detectors[j]->SetFiredStripsVector(evNo);

        for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) {
          for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) {

            temp = GetStripCoordinate(detectors[j]->GetPlane(0)->GetFiredStripsVector()[xval],
                                      detectors[j]->GetPlane(1)->GetFiredStripsVector()[yval], detectors[j]->GetZPos());
            temp.Print();

            if (gEve) {
              m.SetDx(temp.x());
              m.SetDy(temp.y());
              m.SetDz(temp.z());
              hittedPixelVector.push_back(new HittedPixel(m));
              Tracking::Singleton::instance()->AddElement(
                  hittedPixelVector[hittedPixelVector.size() - 1]->GetEveGeoShape());
            }
          }
        }
      }
      sleep(fDelay);
      std::cout << "Size : " << hittedPixelVector.size() << std::endl;
      if (hittedPixelVector.size()) {
        for (int i = 0; i < hittedPixelVector.size(); i++) {
          Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());
        }
      }
      hittedPixelVector.clear();
    }
  }

  void *handle(void *ptr) {

    TGeoHMatrix m;
    LinesAngle l;
    Double_t trans[3] = {0., 0., 0.};
    m.SetTranslation(trans);
    m.SetDx(rand() % 50);
    m.SetDy(rand() % 50);
    m.SetDz(75);

    Coordinates c(Tomography::SetupManager::instance()->GetUpperLayerDetectorVector("GLASS"));
    Tracking::Vector3D<double> temp;
    std::vector<HittedPixel *> hittedPixelVector;
    while (true) {

      c.CoGenerator();
      c.SetStrips();
      c.SetStripCoordinates();

      Vector3D<double> temp2 = c.GetSpecificCoordinate(10.);
      Coordinates c2(Tomography::SetupManager::instance()->GetLowerLayerDetectorVector("GLASS"),temp2);
      c2.CoGenerator();
      c2.SetStrips();
      c2.SetStripCoordinates();

      TGeoBBox *shape;
      std::cout << "Size : " << hittedPixelVector.size() << std::endl;
      if (hittedPixelVector.size()) {
        for (int i = 0; i < hittedPixelVector.size(); i++) {
          Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());
        }
      }

      hittedPixelVector.clear();
      // For top half detectors
      for (int detNo = 0; detNo < Tomography::SetupManager::instance()->GetLowerLayerStartIndex(); detNo++) {
        temp = c.GetStripCoordinate(detNo + 1);
        temp.Print();
        m.SetDx(temp.x());
        m.SetDy(temp.y());
        m.SetDz(temp.z());
        // Add some element
        if (gEve) {
          hittedPixelVector.push_back(new HittedPixel(m));
          Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
        }
      }

      // For bottom half detectors
      for (int detNo = Tomography::SetupManager::instance()->GetLowerLayerStartIndex();
           detNo < Tomography::SetupManager::instance()->GetDetectorVector("GLASS").size(); detNo++) {
        temp = c2.GetStripCoordinate(detNo - Tomography::SetupManager::instance()->GetLowerLayerStartIndex() + 1);
        temp.Print();
        m.SetDx(temp.x());
        m.SetDy(temp.y());
        m.SetDz(temp.z());

        // Add some element
        if (gEve) {
          hittedPixelVector.push_back(new HittedPixel(m));
          Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
        }
      }

      std::cout << "Pt : ";
      c.GetCoordinate(1).Print();

      ls = new TEveStraightLineSet();
      ls->SetLineColor(5);
      AddLine(c);
      AddLine(c2);

      Tracking::Singleton::instance()->AddElement(ls);

      std::cout << "Angle : "
                << l.CalculateAngle(c.GetCoordinate(1), c.GetCoordinate(2), c2.GetCoordinate(1), c2.GetCoordinate(2))
                << std::endl;
      sleep(2);
      Tracking::Singleton::instance()->RemoveElement();
    }
  }


Tracking::Vector3D<double> GetStripCoordinate(double x, double y, double z)
    {
        int tmp = 0;
        Vector3D<double> temp;
        temp.SetX(-50. + x*3.125 + 1.5625);
        temp.SetY(-50. + y*3.125 + 1.5625);
        temp.SetZ(z);

        return temp;
    }

void GenerateCoordinates(std::vector<int>xVec,std::vector<int>yVec){
	for(int i=0 ; i<xVec.size() ; i++){
		for(int j=0 ; j<yVec.size() ; j++){
			Tracking::Vector3D<double> coordinates = GetStripCoordinate(xVec[i],yVec[j],50);
			coordinates.Print();
		}
	}
}


void AddLine(Coordinates c) {
    int startDetIndex = 1;
    int lastDetIndex = c.GetLength();
    ls->AddLine(c.GetCoordinate(startDetIndex).x(), c.GetCoordinate(startDetIndex).y(),
                c.GetCoordinate(startDetIndex).z(), c.GetCoordinate(lastDetIndex).x(),
                c.GetCoordinate(lastDetIndex).y(), c.GetCoordinate(lastDetIndex).z());
    AddMarkers(c);
  }

  void AddMarkers(Coordinates c) {
    for (int i = 0; i < c.GetLength(); i++) {
      ls->AddMarker(c.GetCoordinate(i + 1).x(), c.GetCoordinate(i + 1).y(), c.GetCoordinate(i + 1).z());
    }

    ls->SetMarkerSize(1.3);
    ls->SetMarkerStyle(4);
  }

  void RunThread() {
    TThread *mythread = new TThread("My Thread", (void (*)(void *)) & Update::handle, (void *)this);
    mythread->Run();
  }

  void RunThread2() {
      TThread *mythread2 = new TThread("My Thread2", (void (*)(void *)) & Update::handleRoot, (void *)this);
      mythread2->Run();
    }
};
}

#endif
