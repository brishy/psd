#include "InvertebrateChecker.h"
#include "SeaCreature.h" //need full def
#include <algorithm>     //std::equal, std::tolower, std::transform
#include <iostream>      //cout/cerr
#include <string>
#include <vector>        //vector for egg check list

//case-insensitive string compare helper
namespace { //anon namespace for helper
bool iequals(const std::string& a, const std::string& b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a_char, char b_char) {
                      return std::tolower(a_char) == std::tolower(b_char);
                    });
}
} //namespace

//main checking logic for invertebrates
bool InvertebrateChecker::canKeep(const SeaCreature& creature) const {

  const std::string& species = creature.getSpecies();
  float size = creature.getSize(); //size = carapace width/length probably
  bool hasEggs = creature.carriesEggs();

  //checking eggs first if applicable
  //species that need egg check (based on '+' in rules)
  std::vector<std::string> egg_check_species = {"lobster", "crab", "bug", "crayfish"};
  bool check_eggs = false;
  std::string lower_species = species;
  std::transform(lower_species.begin(), lower_species.end(), lower_species.begin(), ::tolower);

  for (const auto& suffix : egg_check_species) {
    //simple check if species name contains keyword
    if (lower_species.find(suffix) != std::string::npos) {
      check_eggs = true;
      break;
    }
  }

  //carrying eggs? must release.
  if (check_eggs && hasEggs) {
    std::cout << " -> Creature (" << species << ") is carrying eggs and must be released." << std::endl;
    return false;
  }

  //--- NSW DPI Rules (Inverts) ---

  //Blue Swimmer Crab / Blue Crab
  if (iequals(species, "Blue Swimmer Crab") || iequals(species, "Blue Crab")) {
    if (size >= 6.5) {
      return true; //size ok, eggs checked above
    } else {
      std::cout << " -> Blue Swimmer Crab is undersized (< 6.5cm)." << std::endl;
      return false;
    }
  }
  //Mud Crab
  else if (iequals(species, "Mud Crab")) {
    if (size >= 8.5) {
      return true; //size ok, eggs checked above
    } else {
      std::cout << " -> Mud Crab is undersized (< 8.5cm)." << std::endl;
      return false;
    }
  }
  //Spanner Crab
  else if (iequals(species, "Spanner Crab")) {
    if (size >= 9.3) {
      return true; //size ok, eggs checked above
    } else {
      std::cout << " -> Spanner Crab is undersized (< 9.3cm)." << std::endl;
      return false;
    }
  }
  //Eastern Rock Lobster (assume "Rock Lobster" means this one)
  else if (iequals(species, "Eastern Rock Lobster") || iequals(species, "Rock Lobster")) {
    if (size >= 10.4 && size <= 18.0) {
      return true; //in slot limit, eggs checked above
    } else if (size < 10.4) {
      std::cout << " -> Eastern Rock Lobster is undersized (< 10.4cm)." << std::endl;
      return false;
    } else { //size > 18.0
      std::cout << " -> Eastern Rock Lobster is oversized (> 18.0cm)." << std::endl;
      return false;
    }
  }
  //Southern Rock Lobster
  else if (iequals(species, "Southern Rock Lobster")) {
    //Sth Rock Lobster - needs gender? PDF unclear. Applying female min size (10.5cm) for all.
    if (size >= 10.5) {
      return true; //size ok, eggs checked above
    } else {
      std::cout << " -> Southern Rock Lobster is undersized (< 10.5cm)." << std::endl;
      return false;
    }
  }
  //Balmain Bug
  else if (iequals(species, "Balmain Bug")) {
    if (size >= 10.0) {
      return true; //size ok, eggs checked above
    } else {
      std::cout << " -> Balmain Bug is undersized (< 10cm)." << std::endl;
      return false;
    }
  }
  //Squid/Cuttlefish - no size limit
  else if (iequals(species, "Squid") || iequals(species, "Calamari") || iequals(species, "Cuttlefish")) {
    return true;
  }
  //Octopus - no size limit
  else if (iequals(species, "Octopus")) {
    //NOTE: location rules (rock platforms) NOT checked here!
    return true;
  }


  //unknown species
  else {
    std::cout << " -> Species '" << species << "' not found in invertebrate rules. Defaulting to RELEASE." << std::endl;
    return false;
  }
}