//This will be a simple goemetry just to check and see if I can get it working

#include "DetectorConstruction.hh"
//include header files for all classes used in the methods
#include "globals.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Isotope.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

//for sensitive detector definition
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

//constructor / destructor do nothing
DetectorConstruction::DetectorConstruction(){

}

DetectorConstruction::~DetectorConstruction(){ 

}


G4VPhysicalVolume* DetectorConstruction::Construct(){
/* materials definition */
G4NistManager* man = G4NistManager::Instance();

/*define the elements that will be used in our materials*/
G4Element *elLa = man->FindOrBuildElement("La");
G4Element *elBr = man->FindOrBuildElement("Br");

/*define the NIST materials that will be used*/
G4Material* air = man->FindOrBuildMaterial("G4_AIR");
G4Material* Al = man->FindOrBuildMaterial("G4_Al");
  
//constructor of the G4Material class requires arguments: string containing name of material, density, number of elements, state (enum), temperature, pressure
//Lanthanum Bromide Crystal
G4Material* LaBr3 = new G4Material("LanthanumBromide", 5.08 * g / cm3, 2);
LaBr3->AddElement(elLa,1);
LaBr3->AddElement(elBr,3);


/*the volumes: */
/*first we define the world volume -- a box of air, bigger than the crystal*/

//the whole simulation must be contained within a "world" volume
//defining a volume requires definition of solid, logical, and physical volume

//solid volume is the shape, has dimensions
G4double world_sidelength = 100 * cm;

//G4Box defines a box. Arguments are string name, then half-lengths of x/y/z.
//Note that G4 shapes typically work with half lengths, rather than full side
//lengths. Other shapes are possible.
G4Box* world = new G4Box("world", world_sidelength/2, world_sidelength/2,
			  world_sidelength/2);

//logical volume: has a solid volume as a member, a material,pointer to field, sensitive detector,
//G4UserLimits, and flag to optimise. 
G4LogicalVolume* logical_world = new G4LogicalVolume(world, air, "world_log", 0,0,0,true);

G4VisAttributes* VA = new G4VisAttributes();
//hide the world volume, so it doesn't obscure the daughter volumes in the GUI
VA->SetVisibility(false);
logical_world->SetVisAttributes(VA);

//physical volume: G4PVPlacement class, represents a logical volume that is placed somewhere in the mother volume. Arguments: 
G4VPhysicalVolume* physical_world = new G4PVPlacement(0,G4ThreeVector(),logical_world, "world_phys", 0, false, 0);

//G4Tubs has arguments: name (str), inner radius, outer radius, half-length, start theta, end theta.
//These are hard-coded for now, but can be parameterised and adjusted in a macro with a Messenger class
G4double rCrystal = 2.54*cm;
G4double tEnclosure = 2*mm;
G4double lCrystal = 2*2.54*cm;
G4Tubs* enclosure = new G4Tubs("enclosure", 0, rCrystal + tEnclosure, 
		lCrystal/2 + tEnclosure, 0, 360*deg);

G4LogicalVolume* encl_log = new G4LogicalVolume(enclosure, Al, "encl_log", 0, 0, 0,true);

//PVPlacement arguments: rotation matrix ptr, translation vector ptr, logical volime ptr, name string, mother logical volume ptr, copied?, copy number, check surface flag.
//There's actually other ways to instanciate -- check Doxygen.
G4VPhysicalVolume* encl_phys = new G4PVPlacement(0, G4ThreeVector(0,0,20*cm), encl_log,
		"encl_phys", logical_world, 0, false, 0);


G4Tubs* crystal = new G4Tubs("crystal", 0, rCrystal, lCrystal/2, 0, 360*deg);

G4LogicalVolume* crys_log = new G4LogicalVolume(crystal, LaBr3, "crystal_log", 
		0, 0, 0,true);

//create SensitiveDetector object
SensitiveDetector* SD = new SensitiveDetector("SD");
//get pointer to the sensitive detector manager: this class is used by G4RunManager to see which sensitive detectors there are
G4SDManager* SDman = G4SDManager::GetSDMpointer();
//pass to manager
SDman->AddNewDetector(SD);

//now we pass the sensitive detector pointer to the logical volume of our scoring volume
crys_log->SetSensitiveDetector(SD);

G4VisAttributes* VAcrystal = new G4VisAttributes();
//RGB values to set the colour (1,1,1)=white
VAcrystal->SetColour(1,1,1);
crys_log->SetVisAttributes(VA);

G4VPhysicalVolume* crys_phys = new G4PVPlacement(0, G4ThreeVector(), crys_log,
		"crys_phys", encl_log, 0, false, 0); 

//Always need to return the physical world to the run manager
return physical_world;

}
