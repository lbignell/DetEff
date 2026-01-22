#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

//---------------------------------------------------------------------------//

#include "globals.hh"
#include "G4UImessenger.hh"

//---------------------------------------------------------------------------//

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class RunAction;

//---------------------------------------------------------------------------//

class DetectorConstructionMessenger : public G4UImessenger
{
public:
  //default constructor
  DetectorConstructionMessenger(DetectorConstruction *DC);

  //destructor
  ~DetectorConstructionMessenger();

  //public interface
  void SetNewValue(G4UIcommand *command, G4String newValues);

  //protected members
protected:
  //private  members
private:
  DetectorConstruction *fDetectorConstruction;
  G4UIdirectory *fDirectory;
  G4UIcmdWithAString *fSetMatCmd;
};
#endif
