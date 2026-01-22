#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

//---------------------------------------------------------------------------//

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction *DC) : fDetectorConstruction(DC)
{
	//Creates a directory for macro commands under /DetEff/
	fDirectory = new G4UIdirectory("/DetEff/");

	//New command with string argument to modify the output file name from the default	
	fSetMatCmd = new G4UIcmdWithAString("/DetEff/setScintillator", this);
      	fSetMatCmd->SetGuidance("Set the scintillator material. Valid options are either LaBr3 or NaI");
      	fSetMatCmd->SetGuidance("(default: LaBr3)");
	fSetMatCmd->SetCandidates("LaBr3 NaI");
      	fSetMatCmd->SetParameterName("choice", false);

}

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
  delete fSetMatCmd;
}

//---------------------------------------------------------------------------//

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command,
                                          G4String newValue)
{
  if (command == fSetMatCmd)
  {
    fDetectorConstruction->SetScintMat(newValue);
    //Important step: tell the RM about this change and reinitialize geometry.
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    auto* ui = G4UImanager::GetUIpointer();
    ui->ApplyCommand("/run/reinitializeGeometry");
  }
}
