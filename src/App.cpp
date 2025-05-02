#include "App.h"
#include "InvertebrateChecker.h" 
#include "SeaCreatureData.h"
#include "SeaCreatureFactory.h" 
#include "SeaPlusPlusEngine.h" 
#include "VertebrateChecker.h" 

#include <iostream>
#include <limits>    
#include <stdexcept> 
#include <string>


//Here i put here for convenience, normally need a factory for its own files.
#include "InvertebrateCreature.h"
#include "SeaCreature.h"
#include "VertebrateCreature.h"

class ConcreteSeaCreatureFactory : public SeaCreatureFactory {
public:
  std::unique_ptr<SeaCreature>
  createSeaCreature(const SeaCreatureData& details) const override {
    if (details.category == "Vertebrate") {
      return std::make_unique<VertebrateCreature>(details);
    } else if (details.category == "Invertebrate") {
      return std::make_unique<InvertebrateCreature>(details);
    }
    //unknown type? blow up.
    throw std::runtime_error("Unknown creature category for factory: " +
                             details.category);
    //or return nullptr instead of throwing? nah.
  }
};
//--- end Factory ---

//--- ctor ---
App::App() {
  std::cout << "App: Initializing..." << std::endl;
  //make checkers
  auto vChecker = std::make_unique<VertebrateChecker>();
  auto iChecker = std::make_unique<InvertebrateChecker>();
  //make engine, give it checkers (moves ownership)
  engine = std::make_unique<SeaPlusPlusEngine>(std::move(vChecker),
                                               std::move(iChecker));
  //make the factory
  factory = std::make_unique<ConcreteSeaCreatureFactory>();

  //optional: register app w/ engine (if needed for callbacks)
  //engine->registerApp(this);
  std::cout << "App: Initialization Complete." << std::endl;
}

//--- dtor ---
//needed bc unique_ptr to fwd declared types Engine/Factory
App::~App() { std::cout << "App: Shutting down." << std::endl; }

//--- runApplication (Facade) ---
void App::runApplication() {
  std::cout << "\n--- Welcome to Sea++ ---" << std::endl;
  //main interaction loop
  while (true) {
    SeaCreatureData catchData = collectCatchDetails();
    if (catchData.species == "quit") {
      break; //leave loop
    }

    //send catch data to engine to figure it out
    bool keepResult = engine->processCatch(catchData);
    displayResult(keepResult); //tell the user

    //loop again prompt is in displayResult kinda
    std::cout << "\nEnter another catch or type 'quit' for species." << std::endl;
  }

  std::cout << "--- Exiting Sea++ ---" << std::endl;
}

//--- collectCatchDetails ---
SeaCreatureData App::collectCatchDetails() {
  SeaCreatureData data;
  std::string categoryInput;

  std::cout << "\nEnter catch details:" << std::endl;

  //get species name
  std::cout << " Species (e.g., Snapper, Lobster, or 'quit'): ";
  std::getline(std::cin >> std::ws, data.species); //std::ws eats leading whitespace/newline

  if (data.species == "quit")
    return data; //bail out

  //get category (V/I) - simple loop validation
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

  //get size (float) - loop validation
  std::cout << " Size (e.g., 30.5): ";
  while (!(std::cin >> data.size)) {
    std::cout << " Invalid input. Please enter a number for size: ";
    std::cin.clear(); //clear cin errors
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore rest of bad line
  }

  //carrying eggs? (y/n) - loop validation
  char eggsInput = ' ';
  while (eggsInput != 'y' && eggsInput != 'n') {
    std::cout << " Carrying eggs (y/n): ";
    std::cin >> eggsInput;
    eggsInput = std::tolower(eggsInput); //force lowercase
  }
  data.hasEggs = (eggsInput == 'y');

  //eat rest of line before next potential getline
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cout << "------------------------" << std::endl;

  return data;
}

//--- displayResult ---
//just prints the verdict, looks nice if it were to be written like this
void App::displayResult(bool canKeep) {
  std::cout << "\n--- Result ---" << std::endl;
  if (canKeep) {
    std::cout << " Verdict: You can KEEP this catch!" << std::endl;
  } else {
    std::cout << " Verdict: You must RELEASE this catch." << std::endl;
  }
  std::cout << "--------------" << std::endl;
}