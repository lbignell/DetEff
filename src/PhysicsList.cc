//I'll try and keep it as simple, general and as organised as possible.
#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"            //Already included in any physics list
#include "G4RadioactiveDecayPhysics.hh" //Already included in Shielding

//EM lists
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmParameters.hh"

//Hadronic lists
#include "G4HadronPhysicsShielding.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4IonPhysics.hh" //Not very precise. There is a better simulation based on QMD
#include "G4IonQMDPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLEND.hh"


#include "G4ProcessManager.hh"

#include "globals.hh"
#include "G4LossTableManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

PhysicsList::PhysicsList(G4int verbose, G4String LEN_model, G4String HadrPhysVariant)
	:  G4VModularPhysicsList()
{
  // Create a modular physics list and register some modules
  SetVerboseLevel(verbose);
  G4LossTableManager::Instance()->SetVerbose(verbose);

  // Default Decay Physics
  RegisterPhysics(new G4DecayPhysics());
  // Default Radioactive Decay Physics
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  //EM Physics: choice is hard-coded here.
  RegisterPhysics(new G4EmStandardPhysics_option4()); //recommended option
  //RegisterPhysics(new G4EmStandardPhysics());
  //RegisterPhysics(new G4EmPenelopePhysics());
  //RegisterPhysics(new G4EmLivermorePhysics());
  //RegisterPhysics(new G4EmLowEPPhysics());

  //Had Physics: infer from arguments.
  size_t find = LEN_model.find("LEND__");
  G4String evaluation;
  if (find != G4String::npos)
  {
    evaluation = LEN_model;
    evaluation.erase(0, find + 6);
    LEN_model = "LEND";
  }

  // Hadron Elastic scattering
  if (LEN_model == "HP")
  {
    RegisterPhysics(new G4HadronElasticPhysicsHP(verbose));
  }
  else if (LEN_model == "LEND")
  {
    RegisterPhysics(new G4HadronElasticPhysicsLEND(verbose, evaluation));
    //G4DataQuestionaire itt(lend);
  }
  else
  {
    G4cout << "Shielding Physics List: Warning!" << G4endl;
    G4cout << "\"" << LEN_model << "\" is not valid for the low energy neutorn model." << G4endl;
    G4cout << "Neutron HP package will be used." << G4endl;
    RegisterPhysics(new G4HadronElasticPhysicsHP(verbose));
  }

  G4HadronPhysicsShielding *hps;
  // Hadron Physics
  if (HadrPhysVariant == "M")
  {
    hps = new G4HadronPhysicsShielding("hInelastic Shielding", verbose, 9.5 * GeV, 9.9 * GeV);
  }
  else
  {
    hps = new G4HadronPhysicsShielding("hInelastic Shielding", verbose, 4.0 * GeV, 5.0 * GeV);
  }
  if (LEN_model == "HP")
  {
    ;
  }
  else if (LEN_model == "LEND")
  {
    hps->UseLEND(evaluation);
  }
  else
  {
    //G4cout << "Shielding Physics List: Warning." <<G4endl;
    //G4cout << "Name of Low Energy Neutron model " << LEN_model << " is invalid." <<G4endl;
    //G4cout << "Will use neutron HP package." <<G4endl;
  }
  RegisterPhysics(hps);

  if (LEN_model == "HP")
  {
    //Activate prodcuton of fission fragments in neutronHP
    char env_ff[] = "G4NEUTRONHP_PRODUCE_FISSION_FRAGMENTS=1";
    putenv(env_ff);
  }

  // Stopping Physics
  RegisterPhysics(new G4StoppingPhysics(verbose));

  // Ion Physics
  //RegisterPhysics( new G4IonPhysics(verbose));//Less accurate
  RegisterPhysics(new G4IonQMDPhysics(verbose));
  RegisterPhysics(new G4IonElasticPhysics(verbose));

  // Neutron tracking cut --> not by default
  // RegisterPhysics( new G4NeutronTrackingCut(verbose));

  //Others
  //RegisterPhysics(new G4OpticalPhysics());
  // Synchroton Radiation & GN Physics
  RegisterPhysics(new G4EmExtraPhysics());

  //  AddStepMax();

  // Em options
  G4EmParameters *emOptions = G4EmParameters::Instance();
  emOptions->SetBuildCSDARange(true);
  emOptions->SetFluo(true);
  emOptions->SetAuger(true);
  emOptions->SetAugerCascade(true);
  emOptions->SetDeexcitationIgnoreCut(true);
  emOptions->SetPixe(true);

  defaultCutValue = 0.001 * mm; //A track that is expected to travel less than defaultCutValue won't be generated and will become an energy deposit
  cutForGamma = defaultCutValue;
  cutForElectron = defaultCutValue;
  cutForPositron = defaultCutValue;
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
}

void PhysicsList::SetCuts()
{
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  //
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  
  if (verboseLevel>0) DumpCutValuesTable();
}
