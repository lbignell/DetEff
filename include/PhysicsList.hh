#ifndef PhysicsList_hh
#define PhysicsList_hh 1
//the physics list class, this class defines all particles and physics processes that you wish to use in the simulation. Also, you must specify 'range cuts' for particles

//we must include the header file which contains class definitions for the base class
//#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"

//define the class and indicate that it inherits from a base class
class PhysicsList : public G4VModularPhysicsList{

public:
//define the constructor and desctructor methods for this class
    PhysicsList(G4int verbose = 1, G4String low_energy_neutron_model = "HP", 
		    G4String HadrPhysVariant = "");
    virtual ~PhysicsList();
    
    //the user MUST create 3 methods in order for this class to function properly
    void ConstructParticle();//defines particles to use
    void ConstructProcess();//defines physics interactions and assigns them to particles
    void SetCuts();//sets the range cut values for all particles

private:
   G4ParticleTable::G4PTblDicIterator *theParticleIterator;
   //production cuts can affect secondaries in thin detectors
   G4double defaultCutValue = 1*mm;
   G4double cutForElectron = 1*mm;
   G4double cutForPositron = 1*mm;
   G4double cutForGamma = 1*mm;
   
};

#endif
