//once again, fairly simple - copied off the UOW tute
#include "PrimaryGeneratorAction.hh"

//include class definition for the particle gun
//#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

//include header file for particle 
#include "Randomize.hh"
#include "G4Event.hh"
#include <math.h>
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(){
//we need to use dynamic memory allocation for the G4ParticleGun object
  particleGPS = new G4GeneralParticleSource();

  // default particle type -- can be changed in the macro with /gps/ commands
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *particle = particleTable->FindParticle("e-");
  particleGPS->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
//free the dynamically allocated memory
  delete particleGPS;

}

//this method will be called by the RunManager at the beginning of each particle history
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
/*if you wish to randomise some aspect of the primary beam, you will need to utilise the Set methods of the G4ParticleGun object to do so*/
  
  G4int numParticles = 1;
  particleGPS->SetNumberOfParticles(numParticles);
  particleGPS->GeneratePrimaryVertex(anEvent);
}
