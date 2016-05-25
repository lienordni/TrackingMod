/*
 * Visualizer.h
 *
 *  Created on: Apr 6, 2016
 *      Author: rsehgal
 */

#ifndef VISUALIZER_TGEO_VISUALIZER_H_
#define VISUALIZER_TGEO_VISUALIZER_H_
#include <list>
#include <memory>
#include <utility>

class TApplication;
class TGeoManager;
class TGeoMatrix;
class TGeoTranslation;
class TGeoShape;
class TGeoVolume;
class TPolyLine3D;
class TGeoMaterial;
class TGeoMedium;

class TGeoShape;
class TGeoBBox;

#include "TPolyMarker3D.h"
#include "base/Global.h"
#include "base/Vector3D.h"

namespace Tracking {

class Visualizer {
private:
 // TApplication *fApp;       // ROOT application used for visualization
  TGeoManager *fGeoManager; // ROOT geometry manager
  std::vector<std::tuple<TGeoVolume*, TGeoTranslation*> > fVolumes;
  TGeoMaterial *matVacuum; //= new TGeoMaterial("Vacuum", 0,0,0);
  TGeoMedium *Vacuum;// = new TGeoMedium("Vacuum",1, matVacuum);
  bool fVerbosity;
  std::list<std::unique_ptr<TPolyMarker3D> > fMarkers;
  std::list<std::unique_ptr<TPolyLine3D> > fLines;
  
public:
  Visualizer();
  virtual ~Visualizer();
  void Show();
  void Show(TGeoVolume *);
  void AddVolume();
  void AddPoint();
  void AddLine();
  void AddVolume( TGeoVolume *rootVolume);
  void AddVolume( TGeoVolume *rootVolume, Vector3D<Precision> p);
  void AddVolume( TGeoShape *shape, Vector3D<Precision> p);
  void AddVolume( TGeoShape *shape);
  TGeoVolume* CreateTGeoVolume(TGeoShape *shape);

  void AddPoint(Vector3D<Precision> p);
//  void AddLine(TPolyLine3D const &line) ;
  void AddLine(Vector3D<Precision> &p0,Vector3D<Precision> &p1);

  template <class ContainerType>
  void AddPoints(ContainerType const &points, int color) {
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


};

} //end of Tracking namespace

#endif /* INC_VISUALIZER_H_ */
