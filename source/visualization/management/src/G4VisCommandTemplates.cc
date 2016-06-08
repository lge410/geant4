// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4VisCommandTemplates.cc,v 1.2 1999/01/09 16:31:18 allison Exp $
// GEANT4 tag $Name: geant4-00-01 $
//
// 
// Visualization Command Messenger Template private functions.
// John Allison  8th September 1997

#include "G4VisCommandTemplates.hh"

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4ios.hh"

// OLD STYLE!!
G4UIcommand* G4VisButtonCommandMessengerRegister (G4String commandName,
						  G4String guidance,
						  G4UImessenger* messenger) {
  G4UIcommand* command;
  G4UIparameter* param;
  command = new G4UIcommand (commandName, messenger);
  command -> SetGuidance (guidance);
  param   =  new G4UIparameter ("Selector", 'c', false);
  param   -> SetDefaultValue  ("?");
  command -> SetParameter     (param);
  return command;
}

// OLD STYLE!!
G4int G4VisButtonCommandMessengerInterpret (G4String newValues) {
  G4String& choice = newValues;
  G4int iSelector = -1;
  if (choice.compareTo ("0") == 0 ||
      choice.compareTo ("off", RWCString::ignoreCase) == 0 ||
      choice.compareTo ("false", RWCString::ignoreCase) == 0) iSelector = 0;
  if (choice.compareTo ("1") == 0 ||
      choice.compareTo ("on",  RWCString::ignoreCase) == 0 ||
      choice.compareTo ("true", RWCString::ignoreCase) == 0) iSelector = 1;
  if (iSelector < 0 || choice.isNull ()) {
    G4cout << "Choice not recognised."
      "\n  Choice is 0 (or off or false) or 1 (or on or true)." << endl;;
  }
  return iSelector;
}