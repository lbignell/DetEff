#include "SensitiveDetector.hh"
#include "G4Step.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"

#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4SteppingManager.hh"
#include <iterator>
#include "G4TrackVector.hh"
#include "G4ThreeVector.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name){
  name = "SD";
}

SensitiveDetector::~SensitiveDetector(){;}

/*This method is invoked at the beginning of each event. The argument of this method is an object of the G4HCofThisEvent class. Hits collections, where hits produced in this particular event are stored, can be associated to the G4HCofThisEvent object in this method. The hits collections associated with the G4HCofThisEvent  object during this method can be used for ``during the event processing'' digitization.*/
void SensitiveDetector::Initialize(G4HCofThisEvent* HCE){
  RunAction* myRunAction = (RunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
  (myRunAction->x0) =0; 
  (myRunAction->y0) =0; 
  (myRunAction->z0) =0; 
  (myRunAction->px0) =0; 
  (myRunAction->py0) =0; 
  (myRunAction->pz0) =0;
  (myRunAction->E0) =0; 
  (myRunAction->PID0) =0;
  (myRunAction->primX) =0; 
  (myRunAction->primY) =0; 
  (myRunAction->primZ) =0;

  Edep = 0;
  TrackID = 0;
  writtenvertex = false;
  writtenFirstInteraction = false;
}

/*This method is invoked by G4SteppingManager when a step is composed in the G4LogicalVolume which has the pointer to this sensitive detector. The first argument of this method is a G4Step  object of the current step. The second argument is a G4TouchableHistory object for the ``Readout geometry'' described in the next section. The second argument is NULL for the case ``Readout geometry'' is not assigned to this sensitive detector. In this method, one or more G4VHit objects should be constructed if the current step is meaningful for your detector.*/
G4bool SensitiveDetector::ProcessHits(G4Step* theStep, G4TouchableHistory*){
  if((~writtenvertex)&&(theStep->GetTrack()->GetTrackID() == 1)){
	  //Write vertex info
  	  RunAction* myRA = (RunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
	  G4ThreeVector vertex = theStep->GetTrack()->GetVertexPosition();
	  G4ThreeVector pvertex = theStep->GetTrack()->GetVertexMomentumDirection();
	  (myRA->x0) = vertex.x();
	  (myRA->y0) = vertex.y();
	  (myRA->z0) = vertex.z();
	  (myRA->px0) = pvertex.x();
	  (myRA->py0) = pvertex.y();
	  (myRA->pz0) = pvertex.z();
	  (myRA->E0) = theStep->GetTrack()->GetVertexKineticEnergy();
	  (myRA->PID0) = theStep->GetTrack()->GetDefinition()->GetPDGEncoding();

	  writtenvertex = true;
  }
  
  Edep += theStep->GetTotalEnergyDeposit();

  if((theStep->GetTotalEnergyDeposit()>0)&&
		  (theStep->GetTrack()->GetTrackID()==1)&&
		  (~writtenFirstInteraction)){
	  //Record the location of the first interaction point in the detector
	  RunAction* myRA = (RunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
	  G4ThreeVector intpoint = theStep->GetPostStepPoint()->GetPosition();
	  (myRA->primX) = intpoint.x();
	  (myRA->primY) = intpoint.y();
	  (myRA->primZ) = intpoint.z();
	  writtenFirstInteraction = true;
  }
  //the return doesn't really matter unless you use the 'official' Geant4 hit collection
  return true;  
}


/*This method is invoked at the end of each event. The argument of this method is the same object as the previous method. Hits collections occasionally created in your sensitive detector can be associated to the G4HCofThisEvent object.*/
void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
  //get run action pointer
  RunAction* myRunAction = (RunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
  (myRunAction->Edep) = Edep;
  //Pass vectors on to RA.
  myRunAction->FillOutput();
}


