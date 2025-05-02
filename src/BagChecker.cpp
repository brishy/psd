#include "BagChecker.h"
#include "Bag.h"
#include "InvertebrateChecker.h" //for checker defs
#include "SeaCreature.h"         //for  creature def
#include "VertebrateChecker.h"   //for checker defs
#include <algorithm>             //std::equal, std::tolower
#include <iostream>              //cout/cerr
#include <memory>
#include <string>
#include <vector>

//case-insensitive string compare helper
namespace {
bool iequals(const std::string& a, const std::string& b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a_char, char b_char) {
                      return std::tolower(a_char) == std::tolower(b_char);
                    });
}
} //namespace

//ctor def
BagChecker::BagChecker(const VertebrateChecker& vc, const InvertebrateChecker& ic)
    : v_checker(vc), i_checker(ic) {
  //std::cout << "BagChecker Created with Checkers." << std::endl; //debug print removed
}

//validate the whole bag
bool BagChecker::validate(const Bag& bag) const {
  //std::cout << "BagChecker: Validating bag..." << std::endl; //debug print removed

  //counters for aggregate limits
  int bream_tarwhine_count = 0;
  int dusky_flathead_count = 0;
  int other_flathead_count = 0;
  int trevallies_count = 0;
  int tuna_count_total = 0;
  int tuna_count_large = 0; //tuna >= 90cm

  const auto& creatures = bag.getAllCreatures();

  //first, check if EACH creature is legal on its own
  for (const auto& creature_ptr : creatures) {
    if (!creature_ptr) continue; //shouldn't happen if Bag::addCreature works right

    bool individually_legal = false;
    const std::string& category = creature_ptr->getCategory();

    if (category == "Vertebrate") {
      individually_legal = v_checker.canKeep(*creature_ptr);
    } else if (category == "Invertebrate") {
      individually_legal = i_checker.canKeep(*creature_ptr);
    } else {
      //shouldn't really happen with the factory pattern
      std::cerr << "ERROR: Bag contains creature with unknown category: "
                << category << std::endl;
      return false; //unknown category = illegal
    }

    //if one's bad, the whole bag is bad
    if (!individually_legal) {
      std::cout << " -> Bag INVALID: Contains individually illegal creature: "
                << creature_ptr->getSpecies()
                << " (Size: " << creature_ptr->getSize()
                << ", Eggs: " << creature_ptr->carriesEggs() << ")" << std::endl;
      return false;
    }
    //--- End Individual Checks ---

    //--- Count for Aggregate Limits ---
    const std::string& species = creature_ptr->getSpecies();
    if (iequals(species, "Bream") || iequals(species, "Tarwhine")) {
      bream_tarwhine_count++;
    } else if (iequals(species, "Dusky Flathead")) {
      dusky_flathead_count++;
    } else if (iequals(species, "Bluespotted Flathead") || iequals(species, "Tiger Flathead")) {
      other_flathead_count++;
    } else if (species.find("Trevally") != std::string::npos || species.find("Trevall") != std::string::npos) {
      //simple check
      trevallies_count++;
    } else if (iequals(species, "Albacore Tuna") ||
               iequals(species, "Bigeye Tuna") ||
               iequals(species, "Longtail Tuna") ||
               iequals(species, "Yellowfin Tuna"))
    //rules excludes Sth Bluefin from this group limit, so we do too
    {
      tuna_count_total++;
      if (creature_ptr->getSize() >= 90.0) { //magic number from PDF
        tuna_count_large++;
      }
    }
  }

  //--- Check Aggregate Bag Rules ---

  //Bream/Tarwhine limit (10 total)
  if (bream_tarwhine_count > 10) {
    std::cout << " -> Bag INVALID: Exceeds Bream/Tarwhine combined limit of 10 (Count: "
              << bream_tarwhine_count << ")" << std::endl;
    return false;
  }

  //Flathead limits (Dusky 5, Others 10)
  if (dusky_flathead_count > 5) {
    std::cout << " -> Bag INVALID: Exceeds Dusky Flathead limit of 5 (Count: "
              << dusky_flathead_count << ")" << std::endl;
    return false;
  }
  if (other_flathead_count > 10) {
    std::cout << " -> Bag INVALID: Exceeds Other Flathead combined limit of 10 (Count: "
              << other_flathead_count << ")" << std::endl;
    return false;
  }

  //Trevallies limit (10 total)
  if (trevallies_count > 10) {
    std::cout << " -> Bag INVALID: Exceeds Trevallies combined limit of 10 (Count: "
              << trevallies_count << ")" << std::endl;
    return false;
  }

  //Tuna limits (5 total, max 2 >= 90cm)
  if (tuna_count_total > 5) {
    std::cout << " -> Bag INVALID: Exceeds Tuna total limit of 5 (Count: "
              << tuna_count_total << ")" << std::endl;
    return false;
  }
  if (tuna_count_large > 2) {
    std::cout << " -> Bag INVALID: Exceeds Tuna large (>=90cm) limit of 2 (Count: "
              << tuna_count_large << ")" << std::endl;
    return false;
  }


  return true; 
}