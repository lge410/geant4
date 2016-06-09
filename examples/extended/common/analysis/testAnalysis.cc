//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id$
// 
/// \file testAnalysis.cc
/// \brief Test program for the analysis common classes

#ifdef G4_USE_HBOOK
#include "ExG4HbookAnalysisManager.hh"
#endif

// test program which only instantiates classes defined in 
// examples/common/analysis

int main()
{
#ifdef G4_USE_HBOOK
  ExG4HbookAnalysisManager* hbookManager = ExG4HbookAnalysisManager::Instance();
  delete hbookManager;
  
  hbookManager = ExG4HbookAnalysisManager::Instance();
  delete hbookManager;
#endif

  return 0;
}

