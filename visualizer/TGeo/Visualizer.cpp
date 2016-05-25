/*
 * Visualizer.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: rsehgal
 */

#include "TApplication.h"
#include "TAxis3D.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
//#include "TGeoTranslation.h"
#include "TGeoShape.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TVirtualPad.h"
#include "TView.h"

#include <iostream>
#include <TGeoVolume.h>
#include "Visualizer.h"
#include "base/Vector3D.h"
#include "TPad.h"

 #include "TGeoShape.h"

#include "TGeoBBox.h"

#define kInfinity 10000

namespace Tracking {


Visualizer::Visualizer():fGeoManager(0) {
  // TODO Auto-generated constructor stub
// TEveManager::Create();
 //fApp = new TApplication("VecGeom Visualizer", NULL, NULL);
 matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
 matVacuum->SetTransparency(50);
 Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
 fVerbosity = true;

}

Visualizer::~Visualizer() {
  // TODO Auto-generated destructor stub
 // delete fApp;
}

void Visualizer::Show(){

  std::cout<<"=============================================="<<std::endl;
  std::cout<<"========= Inside Expected SHOW() ============="<<std::endl;
  std::cout<<"=============================================="<<std::endl;

  TGeoVolume *top = gGeoManager->MakeBox("Top", NULL, kInfinity, kInfinity, kInfinity);
  gGeoManager->SetTopVolume(top);
  for(int i = 0 ; i < fVolumes.size() ; i++){
  top->AddNode(std::get<0>(fVolumes[i]), 1 , std::get<1>(fVolumes[i]));

  }

  top->SetLineColor(kGreen);
  gGeoManager->CloseGeometry();
  #ifndef USE_OGL
  top->Draw();
  #else
  top->Draw("ogl"); //to display the geometry using openGL
  #endif
  
  for (auto &marker : fMarkers) {
    marker->Draw();
  }

  for (auto &line : fLines) {
    line->Draw();
  }

  //
  //TPad::x3d("OPENGL");
  gGeoManager->Export("plane.root");
  //top->Export("planeTop.root");
  //fApp->Run();
}

void Visualizer::Show(TGeoVolume *vol){
  TGeoVolume *top = gGeoManager->MakeBox("Top", NULL, kInfinity, kInfinity, kInfinity);
  gGeoManager->SetTopVolume(top);
  //TGeoVolume *vol = fGeoManager->MakeSphere("SPHERE", NULL, 30, 40, 0, 180, 0, 360);
  top->AddNode(vol, 1);
  gGeoManager->CloseGeometry();
  top->Draw();
  //fApp->Run();
}

void Visualizer::AddVolume( TGeoVolume *rootVolume) {
  fVolumes.push_back(std::make_tuple(
        rootVolume,
        new TGeoTranslation(0., 0., 0.)) ) ;

}

void Visualizer::AddVolume( TGeoVolume *rootVolume, Vector3D<Precision> p) {

  fVolumes.push_back(std::make_tuple(
      rootVolume,
      new TGeoTranslation(p.x(), p.y(), p.z())) ) ;

}

void Visualizer::AddVolume( TGeoShape *shape, Vector3D<Precision> p) {

  fVolumes.push_back(std::make_tuple(
      new TGeoVolume("SHAPE", shape, Vacuum),
      new TGeoTranslation(p.x(), p.y(), p.z())) ) ;

}


void Visualizer::AddVolume( TGeoShape *shape) {

  fVolumes.push_back(std::make_tuple(
      new TGeoVolume("SHAPE", shape, Vacuum),
      new TGeoTranslation(0.,0.,0.)) ) ;

}

TGeoVolume* Visualizer::CreateTGeoVolume(TGeoShape *shape){
  return (new TGeoVolume("SHAPE", shape, Vacuum));
}
/*
template <class ContainerType>
void Visualizer::AddPoints(ContainerType const &points, int color) {
  const int size = points.size();
  TPolyMarker3D *marker = new TPolyMarker3D(size);
  marker->SetMarkerColor(color);
  marker->SetMarkerSize(1);
  marker->SetMarkerStyle(5);
  for (int i = 0; i < size; ++i) {
    //marker->SetNextPoint(points.x(i), points.y(i), points.z(i));
    marker->SetNextPoint(points[i].x(), points[i].y(), points[i].z());
  }
  fMarkers.emplace_back(marker);
  if (fVerbosity > 0) {
    std::cout << "Added " << size << " points to Visualizer.\n";
  }
}
*/
void Visualizer::AddPoint(Vector3D<Precision> p){
//std::vector<Vector3D<Precision>> points;
//points.push_back(p);
//AddPoints<std::vector<Vector3D<Precision>>(points,2);
TPolyMarker3D *marker = new TPolyMarker3D();
  marker->SetMarkerColor(2);
  marker->SetMarkerSize(1);
  marker->SetMarkerStyle(5);
  marker->SetNextPoint(p.x(), p.y(), p.z());
  fMarkers.emplace_back(marker);
}
/*
void Visualizer::AddLine(TPolyLine3D const &line) {
  fLines.emplace_back(new TPolyLine3D(line));
  auto GetPoint = [&line] (int index) {
    float *pointArray = line.GetP();
    int offset = 3*index;
    return Vector3D<Precision>(
        pointArray[offset], pointArray[offset+1], pointArray[offset+2]);;
  };
  if (line.GetN() == 2) {
    if (fVerbosity > 0) {
      std::cout << "Added line " << GetPoint(0) << "--" << GetPoint(1)
                << " to Visualizer.\n";
    }
  } else {
    if (fVerbosity > 0) {
      std::cout << "Added line with " << line.GetN()
                << " points to Visualizer.\n";
    }
  }
}
*/
void Visualizer::AddLine(
    Vector3D<Precision> &p0,
    Vector3D<Precision> &p1) {

  TPolyLine3D *line = new TPolyLine3D(2);
  line->SetPoint(0, p0.x(), p0.y(), p0.z());
  line->SetPoint(1, p1.x(), p1.y(), p1.z());
  line->SetLineColor(kBlue);
  fLines.emplace_back(line);
  if (fVerbosity > 0) {
    //std::cout << "Added line " << p0 << "--" << p1 << " to Visualizer.\n";
  }
}


} //end of Tracking namespace
