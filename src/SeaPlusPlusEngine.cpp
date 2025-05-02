#include "SeaPlusPlusEngine.h"
#include "InvertebrateChecker.h" // Need full definitions for unique_ptr destructor
#include "SeaCreatureData.h"
#include "VertebrateChecker.h" // Need full definitions for unique_ptr destructor
// #include "SeaCreature.h"      // Base class header - might not be needed
// directly if using derived types
#include "SeaCreatureFactory.h" // Needed if factory is used here (maybe App uses it?)

// --- FIX: Add includes for the concrete creature types ---
#include "InvertebrateCreature.h"
#include "VertebrateCreature.h"
// --- End of FIX ---

// Include App.h ONLY if you need its full definition, otherwise forward decl is
// fine #include "App.h"
#include <iostream>
#include <stdexcept> // For exceptions

// --- Constructor ---
// Takes ownership of the checkers via std::move
SeaPlusPlusEngine::SeaPlusPlusEngine(
    std::unique_ptr<VertebrateChecker> vChecker,
    std::unique_ptr<InvertebrateChecker> iChecker)
    : vertebrateChecker(std::move(vChecker)),
      invertebrateChecker(std::move(iChecker)),
      app(nullptr) // Initialize optional app pointer
{
  if (!vertebrateChecker || !invertebrateChecker) {
    throw std::invalid_argument("Checkers cannot be null.");
  }
  std::cout << "SeaPlusPlusEngine Created." << std::endl;
}

// --- Destructor ---
// Needs to be defined here where concrete checker types are known
SeaPlusPlusEngine::~SeaPlusPlusEngine() {
  std::cout << "SeaPlusPlusEngine Destroyed." << std::endl;
}

// --- processCatch Method (Mediator Logic) ---
bool SeaPlusPlusEngine::processCatch(const SeaCreatureData &creatureData) {
  std::cout << "SeaPlusPlusEngine: Processing catch for species '"
            << creatureData.species << "'" << std::endl;

  // Simple logic based on category string (could be more robust)
  // Note: In a real scenario, you might need a Factory here to create
  // the actual SeaCreature object to pass to the checker,
  // or modify checkers to accept SeaCreatureData.
  // For now, we'll assume category is set and directly call checker.
  // THIS IS A SIMPLIFICATION - NEEDS REFINEMENT BASED ON FACTORY USE

  if (creatureData.category == "Vertebrate") {
    std::cout << " Forwarding to VertebrateChecker..." << std::endl;
    // Simplified: Creating a temporary creature object - better handled via
    // Factory pattern usage in App
    VertebrateCreature tempCreature(
        creatureData); // Now compiler knows this type
    return vertebrateChecker->canKeep(tempCreature); // Pass the object

  } else if (creatureData.category == "Invertebrate") {
    std::cout << " Forwarding to InvertebrateChecker..." << std::endl;
    // Simplified: Creating a temporary creature object - better handled via
    // Factory pattern usage in App
    InvertebrateCreature tempCreature(
        creatureData); // Now compiler knows this type
    return invertebrateChecker->canKeep(tempCreature); // Pass the object

  } else {
    std::cerr << "Warning: Unknown creature category '" << creatureData.category
              << "'" << std::endl;
    return false; // Cannot keep unknown category
  }
}

// --- registerApp Method ---
void SeaPlusPlusEngine::registerApp(App *appInstance) {
  this->app = appInstance;
  std::cout << "SeaPlusPlusEngine: Registered App." << std::endl;
}

// --- registerCheckers (Alternative) ---
// void SeaPlusPlusEngine::registerCheckers(std::unique_ptr<VertebrateChecker>
// vChecker,
//                                         std::unique_ptr<InvertebrateChecker>
//                                         iChecker) {
//     this->vertebrateChecker = std::move(vChecker);
//     this->invertebrateChecker = std::move(iChecker);
// }