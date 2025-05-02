#include "SimpleSeaCreatureFactory.h"
#include "InvertebrateCreature.h" //need concrete types
#include "SeaCreatureData.h"
#include "VertebrateCreature.h"   //need concrete types
#include <iostream>               //cerr
#include <stdexcept>              //runtime_error maybe
#include <string>                 //string compare

//implementation of the factory method
std::unique_ptr<SeaCreature> SimpleSeaCreatureFactory::createSeaCreature(
    const SeaCreatureData& details) const {

  //figure out category from details struct
  std::string category = details.category;

  //make the right creature type based on category string
  if (category == "Vertebrate") {
    return std::make_unique<VertebrateCreature>(details);
  } else if (category == "Invertebrate") {
    return std::make_unique<InvertebrateCreature>(details);
  } else {
    //unknown category? fail.
    //Can either return nullptr or throw, throwing sounds better here
    std::cerr << "ERROR: SimpleSeaCreatureFactory: Unknown creature category '" << category
              << "' for species '" << details.species << "'" << std::endl;
    //throw std::runtime_error("Unknown creature category");
    return nullptr; //return null for now, less noisy
  }
}