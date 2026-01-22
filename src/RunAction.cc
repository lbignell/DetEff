//
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4SDManager.hh"
//
#include "DetectorConstruction.hh"
#include "RunActionMessenger.hh"

#include "G4VSensitiveDetector.hh"
#include <math.h>
#include "TH1D.h"
#include "TFile.h"
#include <sstream>
#include <string>
//#include "G4AnalysisManager.hh"

RunAction::RunAction(DetectorConstruction* DC){
  //
  
  //take the DetectorConstruction pointer given when this object is created (in main) and copy to local member
  myDC = DC;
  //instanciate the RA messenger with a pointer to this object
  pMessenger = new RunActionMessenger(this);
  
}

RunAction::~RunAction(){

}

void RunAction::BeginOfRunAction(const G4Run* aRun){
	//Make the root file.
	outfile = new TFile((fname+".root").c_str(), "RECREATE");

	data = new TTree("data", "tree");

	data->Branch("Edep", &Edep);
	data->Branch("x0", &x0);
	data->Branch("y0", &y0);
	data->Branch("z0", &z0);
	data->Branch("px0", &px0);
	data->Branch("py0", &py0);
	data->Branch("pz0", &pz0);
	data->Branch("E0", &E0);
	data->Branch("PID0", &PID0);

}

void RunAction::FillOutput(){
	//Fill the output tree here.
	data->Fill();
}


//task to be carried out at the end of the run
void RunAction::EndOfRunAction(const G4Run* aRun){
  //Write the output file
  outfile->Write();
	  
  /*//get the number of primary particles being simulated for this run
  G4double NumberOfEvents = aRun->GetNumberOfEventToBeProcessed();
  */
}
