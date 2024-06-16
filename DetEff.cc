/*a Geant4 application requires the use of in built GEANT4 classes, and user defined classes which inherit behaviour from GEANT4 base classes. 
Anyways, for every class that is used in the main function, we must include the defitinion which is contained in the corresponding header file: className.hh
See below for an explanation of what each class does
*/
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4UIExecutive.hh"
//
#include "G4VisExecutive.hh"
//
#include <time.h>
#include "Randomize.hh"

#include "G4Timer.hh"

/*the main function, every C++ program has one*/
int main(int argc, char** argv){
 
  G4Timer* theTimer = new G4Timer();
  //Start the benchmark
  theTimer->Start();
  
  /*//uncomment below to make the random number seed different for each RUN
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  //apparently has a resolution of 1 sec
  time_t clocktime = std::time(NULL);
  CLHEP::HepRandom::setTheSeed(clocktime);*/
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  //get the run manager pointer
  G4RunManager* rm = new G4RunManager();

  //detector construction pointer
  DetectorConstruction* detector = new DetectorConstruction();  
  //register detector
  rm->SetUserInitialization(detector);

  //point to physics list
  G4VUserPhysicsList* physics = new PhysicsList();
  //register it
  rm->SetUserInitialization(physics);

  //Other user initialisations are handled by ActionInitialization.
  rm->SetUserInitialization(new ActionInitialization(detector));
  
  //INITIALISE!!
  rm->Initialize();

  //A second argument means run interactively.
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
      ui = new G4UIExecutive(argc, argv);
  }
  // Get the pointer to the User Interface manager
  G4VisManager* visManager = new G4VisExecutive();
  G4UImanager * UIman = G4UImanager::GetUIpointer();
  //G4UIsession* session = new G4UIterminal(new G4UItcsh); //old school way.

  if(ui){
	  /*now create an instance of the G4VisManager class and intialise the object*/
	  visManager->Initialize();
	  /*this sets up the user interface to run in interactive mode */
	  //now, we run in interactive mode so tell the UI manager to read the vis.mac macro file 
	  UIman->ApplyCommand("/control/execute vis.mac");
	  ui->SessionStart();
    } else { 
	//otherwise we run in batch mode
	G4String command = "/control/execute ";//create first part of command
	G4String fileName = argv[1];//second part is the file name that was typed at the command line 
	UIman->ApplyCommand(command+fileName);//join the two and pass to the UI manager
    }

/*the memory that was dynamically allocated for the run manager must be freed*/
 
  delete ui;
  delete visManager;
  rm->~G4RunManager();

  //Stop the benchmark here
  theTimer->Stop();
  G4cout << "The simulation took: " << theTimer->GetRealElapsed() <<" s to run (real time)" << G4endl;
  return 0;
}
