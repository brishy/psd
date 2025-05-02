#include "App.h"
#include "Angler.h" 
#include "Bag.h" 
#include "BagChecker.h" 
#include "SaveLoadManager.h" 
#include "VertebrateChecker.h" 
#include "InvertebrateChecker.h" 
#include "VertebrateCreature.h" 
#include "InvertebrateCreature.h" 
#include <iostream>
#include <memory> //make_unique
#include <optional> //optional

void Extensions() {

    // --- Bag/BagChecker ---
    std::cout << "\n* Bag & BagChecker *" << std::endl;
    Angler userAngler("user");
    //add some fish (using make_unique)
    userAngler.getBag().addCreature(std::make_unique<VertebrateCreature>("Snapper", 35.0f, false)); // Legal
    userAngler.getBag().addCreature(std::make_unique<VertebrateCreature>("Bream", 30.0f, false));   // Legal
    userAngler.getBag().addCreature(std::make_unique<VertebrateCreature>("Bream", 20.0f, false));   // Illegal (undersize)
    std::cout << " Angler Bag Count: " << userAngler.getBag().creatureCount() << std::endl;

    //create checkers needed by BagChecker
    VertebrateChecker vCheck;
    InvertebrateChecker iCheck;
    BagChecker bagCheck(vCheck, iCheck);

    bool isBagValid = bagCheck.validate(userAngler.getBag());
    std::cout << "Is the bag valid (should be false due to undersize Bream)? "
              << (isBagValid ? "Yes (Problem!)" : "No (Correct)") << std::endl;

    // --- Save/Load ---
    std::cout << "\n* Save/Load Manager *" << std::endl;
    SaveLoadManager slm;
    Angler anglerToSave("save_load");
    anglerToSave.getBag().addCreature(std::make_unique<VertebrateCreature>("Tailor", 31.0f, false));
    anglerToSave.getBag().addCreature(std::make_unique<InvertebrateCreature>("Mud Crab", 9.0f, false));
    std::cout << "Saving Angler '" << anglerToSave.getId() << "' with "
              << anglerToSave.getBag().creatureCount() << " creatures..." << std::endl;

    if (slm.save(anglerToSave)) {
        std::cout << " Save successful. Loading..." << std::endl;
        std::optional<Angler> loadedAnglerOpt = slm.load(anglerToSave.getId());
        if (loadedAnglerOpt) {
            Angler& loadedAngler = loadedAnglerOpt.value();
            std::cout << " Loaded Angler '" << loadedAngler.getId() << "' successfully." << std::endl;
            std::cout << " Loaded Bag Count: " << loadedAngler.getBag().creatureCount()
                      << " (Original had " << anglerToSave.getBag().creatureCount() << ")" << std::endl;
        } else {
            std::cerr << " Load failed!" << std::endl;
        }
    } else {
        std::cerr << " Save failed!" << std::endl;
    }

}


int main() {
    Extensions();


    try {
        App seaApp;
        seaApp.runApplication();
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}