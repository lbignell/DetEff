#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "RunAction.hh"
#include "RunActionMessenger.hh"

//---------------------------------------------------------------------------//

RunActionMessenger::RunActionMessenger(RunAction *runAct) : fRunAction(runAct)
{
	//Creates a directory for macro commands under /DetEff/
	fDirectory = new G4UIdirectory("/DetEff/");

	//New command with string argument to modify the output file name from the default	
	fOutFileCmd = new G4UIcmdWithAString("/DetEff/outfile", this);
      	fOutFileCmd->SetGuidance("Set the output file name without extension");
      	fOutFileCmd->SetGuidance("(default: out)");
      	fOutFileCmd->SetParameterName("choice", false);

}

RunActionMessenger::~RunActionMessenger()
{
  delete fOutFileCmd;
}

//---------------------------------------------------------------------------//

void RunActionMessenger::SetNewValue(G4UIcommand *command,
                                          G4String newValue)
{
  if (command == fOutFileCmd)
  {
    fRunAction->SetOutFile(newValue);
  }//If we had more commands, we could add them as so:
  /*else if (command == fAnotherCommand)
  {
    fRunAction->TheOtherCmd(fAnotherCommand->GetNewDoubleValue(newValue));
  }*/
}
