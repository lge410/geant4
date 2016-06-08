// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4PrimaryTransformer.cc,v 1.1 1999/01/07 16:06:39 gunter Exp $
// GEANT4 tag $Name: geant4-00-01 $
//

#include "G4PrimaryTransformer.hh"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4DecayProducts.hh"
#include "G4ios.hh"

G4PrimaryTransformer::G4PrimaryTransformer()
:verboseLevel(0)
{
  particleTable = G4ParticleTable::GetParticleTable();
}

G4PrimaryTransformer::~G4PrimaryTransformer()
{;}
    
G4TrackVector* G4PrimaryTransformer::GimmePrimaries(G4Event* anEvent)
{
  TV.clearAndDestroy();
  G4int n_vertex = anEvent->GetNumberOfPrimaryVertex();
  if(n_vertex==0) return NULL; 
  for( int i=0; i<n_vertex; i++ )
  { GenerateTracks( anEvent->GetPrimaryVertex(i) ); }
  return &TV;
}

void G4PrimaryTransformer::GenerateTracks(G4PrimaryVertex* primaryVertex)
{
  G4double X0 = primaryVertex->GetX0();
  G4double Y0 = primaryVertex->GetY0();
  G4double Z0 = primaryVertex->GetZ0();
  G4double T0 = primaryVertex->GetT0();

#ifdef G4VERBOSE
  if(verboseLevel>1)
  { 
    G4cout << "G4PrimaryTransformer::PrimaryVertex ("
           << X0 / mm << "(mm),"
           << Y0 / mm << "(mm),"
           << Z0 / mm << "(mm),"
           << T0 / nanosecond << "(nsec))" << endl;
  }
#endif

  G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
  while( primaryParticle != NULL )
  {
    GenerateSingleTrack( primaryParticle, X0, Y0, Z0, T0 );
    primaryParticle = primaryParticle->GetNext();
  }
}

void G4PrimaryTransformer::GenerateSingleTrack
     (G4PrimaryParticle* primaryParticle,
      G4double x0,G4double y0,G4double z0,G4double t0)
{
  G4ParticleDefinition* partDef = GetDefinition(primaryParticle);
  if((!partDef)||partDef->IsShortLived())
  // The particle is not defined in GEANT4, check daughters
  {
#ifdef G4VERBOSE
    if(verboseLevel>2)
    {
      G4cout << "Primary particle (PDGcode " << primaryParticle->GetPDGcode()
             << ") --- Ignored" << endl;
    }
#endif 
    G4PrimaryParticle* daughter = primaryParticle->GetDaughter();
    while(daughter)
    {
      GenerateSingleTrack(daughter,x0,y0,z0,t0);
      daughter = daughter->GetNext();
    }
  }

  // The particle is defined in GEANT4
  else
  {
    // Create G4DynamicParticle object
#ifdef G4VERBOSE
    if(verboseLevel>1)
    {
      G4cout << "Primary particle (" << partDef->GetParticleName()
             << ") --- Transfered with momentum " << primaryParticle->GetMomentum()
             << endl;
    }
#endif
    G4DynamicParticle* DP = 
      new G4DynamicParticle(partDef,primaryParticle->GetMomentum());
    DP->SetPolarization(primaryParticle->GetPolX(),
                        primaryParticle->GetPolY(),
                        primaryParticle->GetPolZ());
    // Set decay products to the DynamicParticle
    SetDecayProducts( primaryParticle, DP );
    // Create G4Track object
    G4Track* track = new G4Track(DP,t0,G4ThreeVector(x0,y0,z0));
    // Set parentID to 0 as a primary particle
    track->SetParentID(0);
    // Store it to G4TrackVector
    TV.insert( track );
  }
}

void G4PrimaryTransformer::SetDecayProducts
      (G4PrimaryParticle* mother, G4DynamicParticle* motherDP)
{
  G4PrimaryParticle* daughter = mother->GetDaughter();
  if(!daughter) return;
  G4DecayProducts* decayProducts = motherDP->GetPreAssignedDecayProducts();
  if(!decayProducts)
  {
    decayProducts = new G4DecayProducts(*motherDP);
    motherDP->SetPreAssignedDecayProducts(decayProducts);
  }
  while(daughter)
  {
    G4ParticleDefinition* partDef = GetDefinition(daughter);
    if(!partDef) 
    { 
#ifdef G4VERBOSE
      if(verboseLevel>2)
      {
        G4cout << " >> Decay product (PDGcode " << daughter->GetPDGcode()
               << ") --- Ignored" << endl;
      }
#endif 
      SetDecayProducts(daughter,motherDP);
    }
    else
    {
#ifdef G4VERBOSE
      if(verboseLevel>1)
      {
        G4cout << " >> Decay product (" << partDef->GetParticleName()
               << ") --- Attached with momentum " << daughter->GetMomentum()
               << endl;
      }
#endif
      G4DynamicParticle*DP 
        = new G4DynamicParticle(partDef,daughter->GetMomentum());
      decayProducts->PushProducts(DP);
      SetDecayProducts(daughter,DP);
    }
    daughter = daughter->GetNext();
  }
}



