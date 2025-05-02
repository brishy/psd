// src/BagChecker.cpp
#include "BagChecker.h"
#include "Bag.h"
#include "InvertebrateChecker.h" // Include headers now needed by constructor
#include "SeaCreature.h"         // Need SeaCreature to access details
#include "VertebrateChecker.h"   // Include headers now needed by constructor
#include <algorithm>             // For std::tolower if using iequals
#include <iostream>              // For debug prints (optional)
#include <memory>
#include <string>
#include <vector>

// Helper (can be shared or local via anonymous namespace)
namespace {
bool iequals(const std::string &a, const std::string &b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {
    return std::tolower(a) == std::tolower(b);
  });
}
} // namespace

// Constructor definition
BagChecker::BagChecker(const VertebrateChecker &vc,
                       const InvertebrateChecker &ic)
    : v_checker(vc), i_checker(ic) // Initializer list
{
  std::cout << "BagChecker Created with Checkers." << std::endl;
}

bool BagChecker::validate(const Bag &bag) const {
  std::cout << "BagChecker: Validating bag..."
            << std::endl; // Optional debug print

  int bream_tarwhine_count = 0;
  int dusky_flathead_count = 0; // <-- ADD Counter
  int other_flathead_count = 0; // <-- ADD Counter
  int trevallies_count = 0;     // <-- ADD Counter
  int tuna_count_total = 0;     // <-- ADD Counter
  int tuna_count_large = 0;     // <-- ADD Counter

  const auto &creatures = bag.getAllCreatures();

  for (const auto &creature_ptr : creatures) {
    if (!creature_ptr)
      continue;

    // --- ADD INDIVIDUAL CHECK ---
    bool individually_legal = false;
    if (creature_ptr->getCategory() == "Vertebrate") {
      individually_legal = v_checker.canKeep(*creature_ptr);
    } else if (creature_ptr->getCategory() == "Invertebrate") {
      individually_legal = i_checker.canKeep(*creature_ptr);
    } else {
      std::cout << " -> Bag INVALID: Unknown creature category found: "
                << creature_ptr->getCategory() << std::endl;
      return false; // Unknown category is illegal
    }

    if (!individually_legal) {
      std::cout << " -> Bag INVALID: Contains individually illegal creature: "
                << creature_ptr->getSpecies()
                << " (Size: " << creature_ptr->getSize()
                << ", Eggs: " << creature_ptr->carriesEggs() << ")"
                << std::endl;
      return false; // If any creature is illegal, the whole bag is
    }
    // --- END INDIVIDUAL CHECK ---

    const std::string &species = creature_ptr->getSpecies();

    // --- Count Species for Bag Limits ---
    if (iequals(species, "Bream") || iequals(species, "Tarwhine")) {
      bream_tarwhine_count++;
    } else if (iequals(species, "Dusky Flathead")) { // <-- ADD Check
      dusky_flathead_count++;
    } else if (iequals(species, "Bluespotted Flathead") ||
               iequals(species, "Tiger Flathead")) { // <-- ADD Check
      other_flathead_count++;
    } else if (species.find("Trevally") != std::string::npos ||
               species.find("Trevall") !=
                   std::string::npos) { // <-- ADD Check (Simple contains check)
      // More robust: check against a list of known trevally species names if
      // needed
      trevallies_count++;
    } else if (iequals(species, "Albacore Tuna") || // <-- ADD TUNA Check Block
               iequals(species, "Bigeye Tuna") ||
               iequals(species, "Longtail Tuna") ||
               iequals(species, "Yellowfin Tuna"))
    // Note: Intentionally excluding Southern Bluefin Tuna based on rules PDF
    // structure
    {
      tuna_count_total++;
      if (creature_ptr->getSize() >= 90.0) {
        tuna_count_large++;
      }
    }

  } // End of loop

  //-- -Apply Bag - Level Rules-- -

  // Bream/Tarwhine Rule
  if (bream_tarwhine_count > 10) {
    std::cout << " -> Bag INVALID: Exceeds Bream/Tarwhine combined limit of 10 "
                 "(Count: "
              << bream_tarwhine_count << ")" << std::endl;
    return false;
  }

  // Flathead Rules [cite: 13] <-- ADD Checks
  if (dusky_flathead_count > 5) {
    std::cout << " -> Bag INVALID: Exceeds Dusky Flathead limit of 5 (Count: "
              << dusky_flathead_count << ")" << std::endl;
    return false;
  }
  if (other_flathead_count > 10) {
    std::cout << " -> Bag INVALID: Exceeds Other Flathead combined limit of 10 "
                 "(Count: "
              << other_flathead_count << ")" << std::endl;
    return false;
  }

  // TODO: Add checks for Trevallies limit (> 10) [cite: 16]
  // Trevallies Rule [cite: 16] <-- ADD Check
  if (trevallies_count > 10) {
    std::cout
        << " -> Bag INVALID: Exceeds Trevallies combined limit of 10 (Count: "
        << trevallies_count << ")" << std::endl;
    return false;
  }
  // TODO: Add checks for Tuna limits [cite: 16]
  // Tuna Rules [cite: 16] <-- ADD Checks
  if (tuna_count_total > 5) {
    std::cout << " -> Bag INVALID: Exceeds Tuna total limit of 5 (Count: "
              << tuna_count_total << ")" << std::endl;
    return false;
  }
  if (tuna_count_large > 2) {
    std::cout
        << " -> Bag INVALID: Exceeds Tuna large (>=90cm) limit of 2 (Count: "
        << tuna_count_large << ")" << std::endl;
    return false;
  }

  // TODO: Consider General Bag Limit? (Skipping as requested)

  std::cout
      << " -> Bag VALID (all individuals legal and checked bag rules pass)."
      << std::endl;
  return true;
  // TODO: Consider General Bag Limit? [cite: 2]

  std::cout
      << " -> Bag VALID (all individuals legal and checked bag rules pass)."
      << std::endl;
  return true; // Passes all checks implemented so far
}