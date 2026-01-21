#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

#include "RunAction.hh"
#include "RunActionMessenger.hh"

//---------------------------------------------------------------------------//

RunActionMessenger::RunActionMessenger(RunAction *runAct) : fRunAction(runAct)
{
  fDirectory = new G4UIdirectory("/DetEff/");

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
  }
  /*else if (command == fAnotherCommand)
  {
    fRunAction->TheOtherCmd(fAnotherCommand->GetNewDoubleValue(newValue));
  }*/
}
