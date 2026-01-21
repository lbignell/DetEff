#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class RunAction;

//---------------------------------------------------------------------------//

class RunActionMessenger : public G4UImessenger
{
public:
  //default constructor
  RunActionMessenger(RunAction *runAct);

  //destructor
  ~RunActionMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //protected members
protected:
  //private  members
private:
  RunAction *fRunAction;
  G4UIdirectory *fDirectory;
  G4UIcmdWithAString *fOutFileCmd;
};
#endif
