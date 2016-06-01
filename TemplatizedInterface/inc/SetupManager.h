/*
 * Scintillator.h
 *
 *  Created on: Mar 30, 2016
 *      Author: rsehgal
 */

#ifndef INC_SETUPMANAGER_H_
#define INC_SETUPMANAGER_H_

//#include "Component.h"
#include <vector>
#include "Scintillator.h"
//#include "RPC.h"
#include "CmsRpc.h"
#include "GlassRpc.h"
#include "TriggeringPlane.h"
#include "Properties.h"
/*
#include "TThread.h"
#include <TEveGeoShape.h>
#include <TGeoBBox.h>
#include <TGeoMatrix.h>
#include <cstdlib>
#include <ctime>
#include "Eve/Singleton.h"
//#include "Coordinates.h"
#include "HittedPixel.h"
*/
typedef Tomography::Properties Detector;

 namespace Tomography{

    
    class SetupManager{
      static SetupManager *s_instance;
      int count;
      //TEveGeoShape *fEveShape;
    private:
        std::vector<Detector*> fTriggeringPlaneVector;
        std::vector<Detector*> fCmsRpcVector;
        std::vector<Detector*> fGlassRpcVector;
        SetupManager(){count=0;}
    public:
        //SetupManager(){count=0;}
        void Register(Detector *det){
          if(det->GetDetectorType().compare("CMS") == 0)
            fCmsRpcVector.push_back(det);

          if(det->GetDetectorType().compare("GLASS") == 0)
            fGlassRpcVector.push_back(det);

          if(det->GetDetectorType().compare("TRG") == 0)
            fTriggeringPlaneVector.push_back(det);

        }

        std::vector<Detector*> GetDetectorVector(std::string detType){
          if(detType.compare("CMS")==0)
            return fCmsRpcVector;
          if(detType.compare("GLASS")==0)
            return fGlassRpcVector;
          if(detType.compare("TRG")==0)
            return fTriggeringPlaneVector;
        }

        static SetupManager *instance();
/*
#ifdef USE_EVE
        void *handle(void *ptr) {

          TGeoHMatrix m;
          Double_t trans[3] = {0., 0., 0.};
          m.SetTranslation(trans);
          m.SetDx(rand()%50);
          m.SetDy(rand()%50);
          m.SetDz(75);

          Coordinates c;
          double* temp;
          std::vector<HittedPixel*> hittedPixelVector;
          while(true){
            sleep(2);

            c.CoGenerator(2, 30);
            c.SetStrips();
            c.SetStripCoordinates();
            //temp = c.GetStripCoordinate(3);
            //std::cout << *temp << " " << *(temp + 1) << " " << *(temp + 2) << std::endl;

            count++;

            TGeoBBox *shape ;

            std::cout<<"Size : "<< hittedPixelVector.size() << std::endl;
            if(hittedPixelVector.size()){
              //std::cout<<"Entered Here"<< std::endl;
              for(int i = 0 ; i < hittedPixelVector.size() ; i++){
                Tracking::Singleton::instance()->RemoveElement(hittedPixelVector[i]->GetEveGeoShape());
              }
            }

            hittedPixelVector.clear();
            for(int detNo = 0 ; detNo < fGlassRpcVector.size() ; detNo++){
              temp = c.GetStripCoordinate(detNo+1);
              std::cout << *temp << " " << *(temp + 1) << " " << *(temp + 2) << std::endl;
            m.SetDx(*temp);
            m.SetDy(*(temp + 1));
            m.SetDz(*(temp + 2));

            //Add some element
            if(gEve){
              hittedPixelVector.push_back(new HittedPixel(m));
              Tracking::Singleton::instance()->AddElement(hittedPixelVector[detNo]->GetEveGeoShape());
             }
            }
        }
        }

        void RunThread() {
          TThread *mythread = new TThread("My Thread", (void (*)(void *)) & SetupManager::handle, (void *)this);
          mythread->Run();
        }
#endif
*/
/*
        template<typename Type,bool ForRpc>
        void Register(Type *component){
            if(ForRpc)
              fRpcVector.push_back(component);
            else
              fScintVector.push_back(component);
        }

        void RegisterScintillatorPlane(ScintillatorPlane *scintPlane){
            fScintVector.push_back(scintPlane);
        }

        void RegisterRpc(RPC *rpc){
            fRpcVector.push_back(rpc);
        }

        void RegisterRpc(CmsRpc *rpc){
                    fCmsRpcVector.push_back(rpc);
                }

        //int GetSize(){return fComponents.size();}


        void GetComponentsName(){
            for(int i = 0 ; i < fScintVector.size() ; i++){
                std::cout<<" Name : " << fScintVector[i]->GetName() << std::endl;
            }
            for(int i = 0 ; i < fRpcVector.size() ; i++){
                std::cout<<" Name : " << fRpcVector[i]->GetName() << std::endl;
            }
        }

        void PrintStrips(){
            for(int i = 0 ; i < fRpcVector.size() ; i++){
                fRpcVector[i]->PrintStrips();
            }
            std::cout<<"Num of Strip in Rpc : " << fRpcVector[0]->GetRpc()->GetNumOfScintillators() << std::endl;
        }




        //-------------------------------------------------

        void *handleCmsRpc(void *ptr) {
          int evNo = 0;
          Tree t("6147.root", "BSC_DATA_TREE");
          while (true) {
            std::cout << std::endl << "------------ Event No : " << evNo << " --------------" << std::endl;
            sleep(3);
            for (int j = 0; j < fScintVector.size(); j++) {
              Reset(j);
              std::cout << std::endl
                        << "Plane Name : " << fScintVector[j]->GetName()
                        << " :: PlaneSize : " << fScintVector[j]->GetNumOfScintillators() << std::endl;
              bool showerEvent = fScintVector[j]->IsShowerEvent<false>(t, evNo);
              std::cout << "ShowerEvent : " << showerEvent << std::endl;

              for (int i = 0; i < fScintVector[j]->GetNumOfScintillators(); i++) {

                bool hit = fScintVector[j]->GetScintillatorPlane()[i]->GetScintHit();
                if (!showerEvent) {
                  if (hit) {
                    std::cout << hit << "  ,  ";
                    fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                  }
                } else {
                  if (hit) {
                    std::cout << hit << "  ,  ";
                    fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(4);
                  }
                }
              }

              std::cout << std::endl;
            }

            for (int j = 0; j < fCmsRpcVector.size(); j++) {
              ResetCmsRpc(j);
//              std::cout << std::endl
//                        << "Plane Name : " << fCmsRpcVector[j]->GetName()
//                        << " :: PlaneSize : " << fCmsRpcVector[j]->GetRpc()->GetNumOfScintillators() << std::endl;
              for (int i = 0; i < fCmsRpcVector[j]->GetEtaA()->GetNumOfScintillators(); i++) {
                fCmsRpcVector[j]->GetEtaA()->GetScintillatorPlane()[i]->DetectAndSetHit<true>(t, evNo);
                bool hit = fCmsRpcVector[j]->GetEtaA()->GetScintillatorPlane()[i]->GetScintHit();
                if (hit) {
                  std::cout << hit << "  ,  ";
                  fCmsRpcVector[j]->GetEtaA()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                }
              }

              for (int i = 0; i < fCmsRpcVector[j]->GetEtaB()->GetNumOfScintillators(); i++) {
                              fCmsRpcVector[j]->GetEtaB()->GetScintillatorPlane()[i]->DetectAndSetHit<true>(t, evNo);
                              bool hit = fCmsRpcVector[j]->GetEtaB()->GetScintillatorPlane()[i]->GetScintHit();
                              if (hit) {
                                std::cout << hit << "  ,  ";
                                fCmsRpcVector[j]->GetEtaB()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                              }
                            }

              for (int i = 0; i < fCmsRpcVector[j]->GetEtaC()->GetNumOfScintillators(); i++) {
                              fCmsRpcVector[j]->GetEtaC()->GetScintillatorPlane()[i]->DetectAndSetHit<true>(t, evNo);
                              bool hit = fCmsRpcVector[j]->GetEtaC()->GetScintillatorPlane()[i]->GetScintHit();
                              if (hit) {
                                std::cout << hit << "  ,  ";
                                fCmsRpcVector[j]->GetEtaC()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                              }
                            }
            }
            evNo++;
          }
        }

        void *handle(void *ptr) {
           int evNo = 0;
           Tree t("6147.root", "BSC_DATA_TREE");
           while (true) {
             std::cout << std::endl << "------------ Event No : " << evNo << " --------------" << std::endl;
             sleep(3);
             for (int j = 0; j < fScintVector.size(); j++) {
               Reset(j);
               std::cout << std::endl
                         << "Plane Name : " << fScintVector[j]->GetName()
                         << " :: PlaneSize : " << fScintVector[j]->GetNumOfScintillators() << std::endl;
               bool showerEvent = fScintVector[j]->IsShowerEvent<false>(t, evNo);
               std::cout << "ShowerEvent : " << showerEvent << std::endl;

               for (int i = 0; i < fScintVector[j]->GetNumOfScintillators(); i++) {

                 bool hit = fScintVector[j]->GetScintillatorPlane()[i]->GetScintHit();
                 if (!showerEvent) {
                   if (hit) {
                     std::cout << hit << "  ,  ";
                     fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                   }
                 } else {
                   if (hit) {
                     std::cout << hit << "  ,  ";
                     fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(4);
                   }
                 }
               }

               std::cout << std::endl;
             }

             for (int j = 0; j < fRpcVector.size(); j++) {
               ResetRpc(j);
 //              std::cout << std::endl
 //                        << "Plane Name : " << fCmsRpcVector[j]->GetName()
 //                        << " :: PlaneSize : " << fCmsRpcVector[j]->GetRpc()->GetNumOfScintillators() << std::endl;
               for (int i = 0; i < fRpcVector[j]->GetRpc()->GetNumOfScintillators(); i++) {
                 fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->DetectAndSetHit<true>(t, evNo);
                 bool hit = fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->GetScintHit();
                 if (hit) {
                   std::cout << hit << "  ,  ";
                   fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(2);
                 }
               }


             }
             evNo++;
           }
         }


        void Reset(int j){
            
                for(int i = 0 ; i < fScintVector[j]->GetNumOfScintillators() ; i++ ){
                    fScintVector[j]->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                }
            
        }

        void ResetRpc(int j){
            
                for(int i = 0 ; i < fRpcVector[j]->GetRpc()->GetNumOfScintillators() ; i++ ){
                    fRpcVector[j]->GetRpc()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                }
            
        }
        void ResetCmsRpc(int j){

                        for(int i = 0 ; i < fCmsRpcVector[j]->GetEtaA()->GetNumOfScintillators() ; i++ ){
                            fCmsRpcVector[j]->GetEtaA()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                        }

                        for(int i = 0 ; i < fCmsRpcVector[j]->GetEtaB()->GetNumOfScintillators() ; i++ ){
                                                    fCmsRpcVector[j]->GetEtaB()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                                                }

                        for(int i = 0 ; i < fCmsRpcVector[j]->GetEtaC()->GetNumOfScintillators() ; i++ ){
                                                    fCmsRpcVector[j]->GetEtaC()->GetScintillatorPlane()[i]->GetScintillatorEveGeoShape()->SetMainColor(3);
                                                }

                }


        void RunThread(){
  
        TThread* mythread= new TThread("My Thread",
           (void(*) (void *))&SetupManager::handle,
           (void*) this);
        mythread->Run();
        }

        void RunCmsThread(){

                TThread* mythread= new TThread("My Thread",
                   (void(*) (void *))&SetupManager::handleCmsRpc,
                   (void*) this);
                mythread->Run();
                }

  */

    };

SetupManager *SetupManager::s_instance = 0;
SetupManager* SetupManager::instance() {
        if (!s_instance)
          s_instance = new SetupManager;
        return s_instance;
    }
}//end of Tracking namespace

#endif
