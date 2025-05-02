#include "SimpleSeaCreatureFactory.h"
#include "InvertebrateCreature.h"
#include "SeaCreatureData.h"
#include "VertebrateCreature.h" // Need concrete classes
#include <iostream>             // For error messages (optional)
#include <stdexcept>            // For exceptions
#include <string>               // For comparing category

std::unique_ptr<SeaCreature> SimpleSeaCreatureFactory::createSeaCreature(
    const SeaCreatureData &details) const {
  // Determine category (assuming it's set in details, or determine it here)
  std::string category =
      details.category; // Or determine based on species, etc.

  // Simple logic to create the correct type based on category
  if (category == "Vertebrate") {
    return std::make_unique<VertebrateCreature>(details);
  } else if (category == "Invertebrate") {
    return std::make_unique<InvertebrateCreature>(details);
  } else {
    // Handle unknown category - throw exception or return nullptr
    std::cerr << "Error: Unknown creature category '" << category
              << "' for species '" << details.species << "'" << std::endl;
    // Or throw std::runtime_error("Unknown creature category");
    return nullptr;
  }
}