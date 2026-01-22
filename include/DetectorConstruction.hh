//preprocessor directives to prevent multiple includes of the same file
#ifndef DetectorConstruction_H 
#define DetectorConstruction_H 1

//preprocessor directive to include the header file for the base class
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

//class declaration, we don't need to tell the compiler how big it is, yet
class G4VPhysicalVolume;
class DetectorConstructionMessenger;

//this is the user defined class that inherits behaviour from the virtual base class of the geant4 toolkit
class DetectorConstruction : public G4VUserDetectorConstruction{

private:
G4String fScintName = "LaBr3";
G4Material* fScintMat = nullptr;
DetectorConstructionMessenger* fDetMessenger = nullptr;

public:
//declare the constructor for this class
    DetectorConstruction();
//declare the destructor of this class
    ~DetectorConstruction();

//the one method that MUST be defined in this class, it is called "Construct", takes no arguments, and returns a pointer to an object of type G4VPhysicalVolume
    G4VPhysicalVolume* Construct();

    void SetScintMat(const G4String& name){fScintName = name;}

};
#endif
