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
#include "Eve/Singleton.h"
#include "VisualizationHelper.h"
#include "TThread.h"
#include "LinesAngle.h"
#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Tree.h"
#include "base/Vector3D.h"

namespace Tomography {

class Update {

  TEveStraightLineSet *ls;

public:
  Update() {}

  void *handleRoot(void *ptr) {
	  TGeoHMatrix m;
	      LinesAngle l;
	      Double_t trans[3] = {0., 0., 0.};
	      m.SetTranslation(trans);
	      m.SetDx(rand() % 50);
	      m.SetDy(rand() % 50);
	      m.SetDz(75);
	      Tracking::Tree::instance()->ReadTree("6742.root", "BSC_DATA_TREE", 0);
	      int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
	      Tracking::Vector3D<double> temp;
	      std::vector<HittedPixel *> hittedPixelVector;

              for (int evNo = 0; evNo < numOfEvents; evNo++) {

                TGeoBBox *shape;
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

    Coordinates c;
    Tracking::Vector3D<double> temp;
    std::vector<HittedPixel *> hittedPixelVector;
    while (true) {

      c.CoGenerator(0);
      c.SetStrips();
      c.SetStripCoordinates();

      Vector3D<double> temp2 = c.GetSpecificCoordinate(10.);
      Coordinates c2(temp2);
      c2.CoGenerator(1);
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

//        temp.SetX(floor( (x+(double)50) /3.125 ));
//        temp.SetY(floor( (y+(double)50) /3.125 ));
        temp.SetX(-50. + x*3.125 + 1.5625);
        temp.SetY(-50. + y*3.125 + 1.5625);
        temp.SetZ(z);

        return temp;
    }

/*
 *
 *          stripcoord[i].SetX(-50. + (strip[i].x())*3.125 + 1.5625);
			stripcoord[i].SetY(-50. + (strip[i].y())*3.125 + 1.5625);
			stripcoord[i].SetZ(coordinate[i].z());
 */

void GenerateCoordinates(std::vector<int>xVec,std::vector<int>yVec){
	for(int i=0 ; i<xVec.size() ; i++){
		for(int j=0 ; j<yVec.size() ; j++){
			Tracking::Vector3D<double> coordinates = GetStripCoordinate(xVec[i],yVec[j],50);
			coordinates.Print();
		}
	}
}

/*  Tracking::Vector3D<double> GetStripCoordinate(double x, double y, double z)
      {
          int tmp = 0;
          Vector3D<double> temp;

  //        temp.SetX(floor( (x+(double)50) /3.125 ));
  //        temp.SetY(floor( (y+(double)50) /3.125 ));
          temp.SetX(-50. + x*3.125 + 1.5625);
          temp.SetY(-50. + y*3.125 + 1.5625);
          temp.SetZ(z);

          return temp;
      }
 */

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
