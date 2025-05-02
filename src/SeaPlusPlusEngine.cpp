//src/SeaPlusPlusEngine.cpp
#include "SeaPlusPlusEngine.h"
#include "InvertebrateChecker.h" //need full def for unique_ptr dtor
#include "InvertebrateCreature.h"//needed for temp object creation
#include "SeaCreatureData.h"
#include "VertebrateChecker.h"   //need full def for unique_ptr dtor
#include "VertebrateCreature.h"  //needed for temp object creation

#include <iostream>  //cout/cerr
#include <stdexcept> //invalid_argument

//--- Ctor ---
//takes ownership of checkers via std::move
SeaPlusPlusEngine::SeaPlusPlusEngine(
    std::unique_ptr<VertebrateChecker> vChecker,
    std::unique_ptr<InvertebrateChecker> iChecker)
    : vertebrateChecker(std::move(vChecker)),
      invertebrateChecker(std::move(iChecker)),
      app(nullptr) //init app ptr
{
  //checkers must be valid
  if (!vertebrateChecker || !invertebrateChecker) {
    throw std::invalid_argument("Engine checkers cannot be null.");
  }
}

//--- Dtor ---
//needed for unique_ptr to fwd declared Checkers
SeaPlusPlusEngine::~SeaPlusPlusEngine() {
}

//--- processCatch (Mediator) ---
//gets data from App, routes to correct checker
bool SeaPlusPlusEngine::processCatch(const SeaCreatureData& creatureData) {

  //route based on category string

  if (creatureData.category == "Vertebrate") {
    //creating temp obj here for now
    VertebrateCreature tempCreature(creatureData);
    return vertebrateChecker->canKeep(tempCreature);
  } else if (creatureData.category == "Invertebrate") {
    //same as above
    InvertebrateCreature tempCreature(creatureData);
    return invertebrateChecker->canKeep(tempCreature);
  } else {
    //reject unknown category
    std::cerr << "Engine Warning: Unknown creature category '" << creatureData.category
              << "' received." << std::endl;
    return false;
  }
}

//--- registerApp ---
void SeaPlusPlusEngine::registerApp(App* appInstance) {
  this->app = appInstance;
}