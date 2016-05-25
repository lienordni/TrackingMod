#include "TGeo/Visualizer.cpp"
#include "base/Vector3D.h"
#include <TGeoShape.h>
#include <TGeoBBox.h>
#include <TApplication.h>

int main(){
   TApplication *fApp = new TApplication("VecGeom Visualizer", NULL, NULL);
   Tracking::Visualizer v;
   TGeoShape *vol = new TGeoBBox("testBox", 100,100,100);
   v.AddVolume(vol);
   Tracking::Vector3D<Precision> pt1(18.2835, 16.6624, 60);
   Tracking::Vector3D<Precision> pt2(13.2067, -10.7788, 30);
   Tracking::Vector3D<Precision> pt3(8.12996, -38.22, 0);
   v.AddPoint(pt1);
   v.AddPoint(pt2);
   v.AddPoint(pt3);

   v.AddLine(pt1,pt3);
   v.Show();
   fApp->Run();

   return 0;

   
}
