// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4PiMinusStopCu.cc,v 1.1 1999/01/07 16:13:45 gunter Exp $
// GEANT4 tag $Name: geant4-00-01 $
//
// -------------------------------------------------------------------
//      GEANT 4 class file --- Copyright CERN 1998
//      CERN Geneva Switzerland
//
//      For information related to this code contact:
//      CERN, IT Division, ASD group
//
//      File name:     G4PiMinusStopCu
//
//      Author:        Maria Grazia Pia (pia@genova.infn.it)
// 
//      Creation date: 8 May 1998
//
//      Modifications: 
// -------------------------------------------------------------------

#include "G4ios.hh"

#include "G4PiMinusStopCu.hh"

#include <rw/tpordvec.h>
#include <rw/tvordvec.h>
#include <rw/cstring.h>

#include "globals.hh"
#include "Randomize.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4ParticleTypes.hh"
#include "G4ReactionKinematics.hh"
#include "G4DynamicParticleVector.hh"
#include "G4LorentzVector.hh"
#include "G4NucleiPropertiesTable.hh"
#include "G4PiMinusStopMaterial.hh"
#include "G4DistributionGenerator.hh"

// np/pp production ratio
// Experimental values: 
// R(np/pp) = 6.3 +- 1.4 (E. Gadioli et al., Phys Rev C36 (1987) 741) 
// [looks odd!]
G4double G4PiMinusStopCu::npRatio = 2.;
 
// Average numbers of final nucleons detected, for N-pair absorption
// (R. Madey  et al., Phys Rev C25 (1982) 3050
G4double G4PiMinusStopCu::nFinalNucleons = 1.72;

// Kinetic energy (MeV) distributions measured for coincident nucleon 
// emission
// (R. Madey  et al., Phys Rev C25 (1982) 3050

G4int G4PiMinusStopCu::eKinEntries = 10;

G4double G4PiMinusStopCu::eKinData[10] = { 0.24, 0.57, 1.13, 1.38,
                                     1.54, 
				     1.38, 1.13, 0.84, 0.57, 0.24};

G4double G4PiMinusStopCu::eKin[11] = { 5.2, 10.2, 19.9, 41.5,
                                  49.6,
				  57.7, 79.3, 94.4, 114., 120., 135.};


// Opening angle distributions measured for coincident nucleon emission
// (P.Heusi et al., Nucl. Phys. A407 (1983) 429

G4int G4PiMinusStopCu::angleEntries = 7;

G4double G4PiMinusStopCu::angleData[7] = 
{ 1.43, 1.67, 2.62, 4.29, 7.62, 11.90, 14.76 };

G4double G4PiMinusStopCu::angle[8] = { 1.308997, 1.570796, 1.832596, 2.094395, 
				  2.356194, 2.617994, 2.967060, 3.1415927 };



// Constructor

G4PiMinusStopCu::G4PiMinusStopCu()
  
{
  // Cluster size: nucleon pair, alpha, triton etc.
  // First implementation: interaction with nucleon pair only
  _clusterSize = 2;

  // R ratio
  _R = 1. / (1. + npRatio);

  _definitions = new RWTPtrOrderedVector<G4ParticleDefinition>();
  _momenta = new RWTPtrOrderedVector<G4LorentzVector>();

  RWTValOrderedVector<double> eKinVector;
  RWTValOrderedVector<double> eKinDataVector;
  int i;
  for (i=0; i<eKinEntries; i++)
    {
      eKinVector.insert(eKin[i]);
      eKinDataVector.insert(eKinData[i]);
    }
  eKinVector.insert(eKin[eKinEntries]);
  _distributionE = new G4DistributionGenerator(eKinVector,eKinDataVector);

  RWTValOrderedVector<double> angleVector;
  RWTValOrderedVector<double> angleDataVector;
  for (i=0; i<angleEntries; i++)
    {
      angleVector.insert(angle[i]);
      angleDataVector.insert(angleData[i]);
    }
  angleVector.insert(angle[angleEntries]);
  _distributionAngle = new G4DistributionGenerator(angleVector,angleDataVector);
}


// Destructor

G4PiMinusStopCu::~G4PiMinusStopCu()
{}

G4double G4PiMinusStopCu::FinalNucleons()
{
  return nFinalNucleons;
}
