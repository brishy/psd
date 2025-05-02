#include "App.h"
#include "InvertebrateChecker.h" // Needed for creating instances
#include "SeaCreatureData.h"
#include "SeaCreatureFactory.h" // Need full definitions for unique_ptr destructor
#include "SeaPlusPlusEngine.h" // Need full definitions for unique_ptr destructor
#include "VertebrateChecker.h" // Needed for creating instances
// Include concrete factory implementation header when you create one
// #include "ConcreteSeaCreatureFactory.h"
#include <iostream>
#include <limits>    // Required for numeric_limits
#include <stdexcept> // Required for runtime_error
#include <string>

// --- Concrete Factory Implementation (Example) ---
// Normally this would be in its own .h/.cpp file (e.g.,
// ConcreteSeaCreatureFactory.h/cpp) Including it here just for demonstration
// purposes.
#include "InvertebrateCreature.h"
#include "SeaCreature.h"
#include "VertebrateCreature.h"

class ConcreteSeaCreatureFactory : public SeaCreatureFactory {
public:
  std::unique_ptr<SeaCreature>
  createSeaCreature(const SeaCreatureData &details) const override {
    if (details.category == "Vertebrate") {
      return std::make_unique<VertebrateCreature>(details);
    } else if (details.category == "Invertebrate") {
      return std::make_unique<InvertebrateCreature>(details);
    }
    // Handle error or unknown type
    throw std::runtime_error("Unknown creature category for factory: " +
                             details.category);
    // return nullptr; // Or return nullptr if preferred over exceptions
  }
};
// --- End of Concrete Factory Example ---

// --- Constructor ---
App::App() {
  std::cout << "App: Initializing..." << std::endl;

  // Create concrete checkers
  auto vChecker = std::make_unique<VertebrateChecker>();
  auto iChecker = std::make_unique<InvertebrateChecker>();

  // Create the engine and give it ownership of the checkers
  engine = std::make_unique<SeaPlusPlusEngine>(std::move(vChecker),
                                               std::move(iChecker));

  // Create a concrete factory
  factory = std::make_unique<ConcreteSeaCreatureFactory>(); // Use your concrete
                                                            // factory

  // Optionally register the app with the engine if needed for callbacks
  // engine->registerApp(this);

  std::cout << "App: Initialization Complete." << std::endl;
}

// --- Destructor ---
// Needs to be defined here where concrete Engine/Factory types are known
App::~App() { std::cout << "App: Shutting down." << std::endl; }

// --- runApplication Method (Facade Logic) ---
void App::runApplication() {
  std::cout << "\n--- Welcome to Sea++ ---" << std::endl;

  // Simple loop for demonstration
  while (true) {
    SeaCreatureData catchData = collectCatchDetails();

    if (catchData.species == "quit") {
      break; // Exit loop
    }

    // Let the engine process the catch
    bool keepResult = engine->processCatch(catchData);

    displayResult(keepResult);

    std::cout << "\nEnter another catch or type 'quit' for species."
              << std::endl;
  }

  std::cout << "--- Exiting Sea++ ---" << std::endl;
}

// --- collectCatchDetails Method ---
SeaCreatureData App::collectCatchDetails() {
  SeaCreatureData data;
  std::string categoryInput;

  std::cout << "\nEnter catch details:" << std::endl;

  // Get Species
  std::cout << " Species (e.g., Snapper, Lobster, or 'quit'): ";
  std::getline(std::cin >> std::ws,
               data.species); // std::ws consumes leftover newline

  if (data.species == "quit")
    return data; // Early exit

  // Get Category (simplified)
  while (categoryInput != "V" && categoryInput != "I") {
    std::cout << " Category (V for Vertebrate / I for Invertebrate): ";
    std::cin >> categoryInput;
    if (categoryInput == "V")
      data.category = "Vertebrate";
    else if (categoryInput == "I")
      data.category = "Invertebrate";
    else
      std::cout << " Invalid category. Please enter V or I." << std::endl;
  }

  // Get Size
  std::cout << " Size (e.g., 30.5): ";
  while (!(std::cin >> data.size)) {
    std::cout << " Invalid input. Please enter a number for size: ";
    std::cin.clear(); // Clear error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // Discard bad input
  }

  // Get Eggs status
  char eggsInput = ' ';
  while (eggsInput != 'y' && eggsInput != 'n') {
    std::cout << " Carrying eggs (y/n): ";
    std::cin >> eggsInput;
    eggsInput = std::tolower(eggsInput);
  }
  data.hasEggs = (eggsInput == 'y');

  // Clear the rest of the input buffer before next loop iteration
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  // TODO: Collect otherRelevantInfo if needed based on your design
  std::cout << "------------------------" << std::endl;

  return data;
}

// --- displayResult Method ---
void App::displayResult(bool canKeep) {
  std::cout << "\n--- Result ---" << std::endl;
  if (canKeep) {
    std::cout << " Verdict: You can KEEP this catch!" << std::endl;
  } else {
    std::cout << " Verdict: You must RELEASE this catch." << std::endl;
  }
  std::cout << "--------------" << std::endl;
}