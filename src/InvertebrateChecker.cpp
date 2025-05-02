#include "InvertebrateChecker.h"
#include "SeaCreature.h" // Include the full definition now
#include <algorithm>     // For std::tolower
#include <iostream>
#include <string>
#include <vector> // For std::vector

// --- FIX: Wrap helper function in anonymous namespace ---
namespace { // Anonymous namespace for internal linkage
bool iequals(const std::string &a, const std::string &b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {
    return std::tolower(a) == std::tolower(b);
  });
}
} // end anonymous namespace
// --- End of FIX ---

bool InvertebrateChecker::canKeep(const SeaCreature &creature) const {
  std::cout << "InvertebrateChecker: Applying NSW DPI rules to species: "
            << creature.getSpecies() << std::endl;

  std::string species = creature.getSpecies();
  float size =
      creature
          .getSize(); // Assumes size is carapace width/length as appropriate
  bool hasEggs = creature.carriesEggs();

  // General Rule: Check for eggs first for applicable species
  // List of species where egg check applies explicitly mentioned (+) in the PDF
  std::vector<std::string> egg_check_species = {"lobster", "crab", "bug",
                                                "crayfish"};
  bool check_eggs = false;
  for (const auto &suffix : egg_check_species) {
    // Simple check if species name contains the keyword (case-insensitive)
    std::string lower_species = species;
    std::transform(lower_species.begin(), lower_species.end(),
                   lower_species.begin(), ::tolower);
    if (lower_species.find(suffix) != std::string::npos) {
      check_eggs = true;
      break;
    }
  }

  if (check_eggs && hasEggs) {
    std::cout << " -> Creature is carrying eggs and must be released."
              << std::endl;
    return false; // Must release if carrying eggs
  }

  // --- NSW DPI Saltwater Rules Implementation (Based on provided PDF) ---
  // (Rules logic remains the same as before)

  // Blue Swimmer Crab
  if (iequals(species, "Blue Swimmer Crab") || iequals(species, "Blue Crab")) {
    // Egg check already performed above
    if (size >= 6.5) {
      std::cout << " -> Blue Swimmer Crab meets minimum size (>= 6.5cm) and "
                   "has no eggs."
                << std::endl;
      return true;
    } else {
      std::cout << " -> Blue Swimmer Crab is undersized (< 6.5cm)."
                << std::endl;
      return false;
    }
  }
  // Mud Crab
  else if (iequals(species, "Mud Crab")) {
    // Egg check already performed above
    if (size >= 8.5) {
      std::cout << " -> Mud Crab meets minimum size (>= 8.5cm) and has no eggs."
                << std::endl;
      return true;
    } else {
      std::cout << " -> Mud Crab is undersized (< 8.5cm)." << std::endl;
      return false;
    }
  }
  // Spanner Crab
  else if (iequals(species, "Spanner Crab")) {
    // Egg check already performed above
    if (size >= 9.3) {
      std::cout
          << " -> Spanner Crab meets minimum size (>= 9.3cm) and has no eggs."
          << std::endl;
      return true;
    } else {
      std::cout << " -> Spanner Crab is undersized (< 9.3cm)." << std::endl;
      return false;
    }
  }
  // Eastern Rock Lobster
  else if (iequals(species, "Eastern Rock Lobster") ||
           iequals(
               species,
               "Rock Lobster")) { // Assuming Rock Lobster defaults to Eastern
    // Egg check already performed above
    if (size >= 10.4 && size <= 18.0) {
      std::cout << " -> Eastern Rock Lobster is within size limits "
                   "(10.4cm-18cm) and has no eggs."
                << std::endl;
      return true;
    } else if (size < 10.4) {
      std::cout << " -> Eastern Rock Lobster is undersized (< 10.4cm)."
                << std::endl;
      return false;
    } else { // size > 18.0
      std::cout << " -> Eastern Rock Lobster is oversized (> 18.0cm)."
                << std::endl;
      return false;
    }
  }
  // Southern Rock Lobster - Requires knowing gender, which we don't have.
  // Simplification: Apply average/stricter rule? Or prompt user?
  // Applying the female minimum size as a general rule.
  else if (iequals(species, "Southern Rock Lobster")) {
    // Egg check already performed above
    if (size >= 10.5) { // Using female minimum as the general minimum here
      std::cout << " -> Southern Rock Lobster meets minimum size (>= 10.5cm) "
                   "and has no eggs."
                << std::endl;
      return true;
    } else {
      std::cout << " -> Southern Rock Lobster is undersized (< 10.5cm)."
                << std::endl;
      return false;
    }
  }
  // Balmain Bug
  else if (iequals(species, "Balmain Bug")) {
    // Egg check already performed above
    if (size >= 10.0) {
      std::cout
          << " -> Balmain Bug meets minimum size (>= 10cm) and has no eggs."
          << std::endl;
      return true;
    } else {
      std::cout << " -> Balmain Bug is undersized (< 10cm)." << std::endl;
      return false;
    }
  }
  // Squid & Cuttlefish (No size limit mentioned)
  else if (iequals(species, "Squid") || iequals(species, "Calamari") ||
           iequals(species, "Cuttlefish")) {
    std::cout
        << " -> Squid/Cuttlefish have no size limit specified. Assuming OK."
        << std::endl;
    return true;
  }
  // Octopus (No size limit mentioned)
  else if (iequals(species, "Octopus")) {
    // NOTE: Location restrictions (rock platforms) are not handled by this
    // logic!
    std::cout << " -> Octopus has no size limit specified. Assuming OK "
                 "(location rules not checked)."
              << std::endl;
    return true;
  }

  // ADD MORE 'ELSE IF' BLOCKS HERE FOR OTHER INVERTEBRATES (Abalone, Bugs,
  // other Crabs, Prawns, etc.)

  // Default case for species not explicitly listed above
  else {
    std::cout
        << " -> Species not found in specific rules. Defaulting to RELEASE."
        << std::endl;
    return false; // Safer default
  }
}