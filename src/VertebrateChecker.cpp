#include "VertebrateChecker.h"
#include "SeaCreature.h" // Include the full definition now
#include <algorithm>     // For std::tolower
#include <iostream>
#include <string>
#include <vector> // Used for checking grouped species if needed

// --- FIX: Wrap helper function in anonymous namespace ---
namespace { // Anonymous namespace for internal linkage
bool iequals(const std::string &a, const std::string &b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {
    return std::tolower(a) == std::tolower(b);
  });
}
} // end anonymous namespace
// --- End of FIX ---

bool VertebrateChecker::canKeep(const SeaCreature &creature) const {
  std::cout << "VertebrateChecker: Applying NSW DPI rules to species: "
            << creature.getSpecies() << std::endl;

  std::string species = creature.getSpecies();
  float size = creature.getSize();
  // bool hasEggs = creature.carriesEggs();

  // --- NSW DPI Saltwater Rules Implementation ---
  // (Rules logic remains the same as before)

  // Bream & Tarwhine Group
  if (iequals(species, "Bream") || iequals(species, "Black Bream") ||
      iequals(species, "Southern Bream") ||
      iequals(species, "Yellowfin Bream")) {
    if (size >= 25.0) {
      std::cout << " -> Bream meets minimum size (>= 25cm)." << std::endl;
      return true;
    } else {
      std::cout << " -> Bream is undersized (< 25cm)." << std::endl;
      return false;
    }
  } else if (iequals(species, "Tarwhine")) {
    if (size >= 20.0) {
      std::cout << " -> Tarwhine meets minimum size (>= 20cm)." << std::endl;
      return true;
    } else {
      std::cout << " -> Tarwhine is undersized (< 20cm)." << std::endl;
      return false;
    }
  }
  // Flathead (Dusky)
  else if (iequals(species, "Flathead") || iequals(species, "Dusky Flathead")) {
    if (size >= 36.0 && size <= 70.0) {
      std::cout << " -> Dusky Flathead is within slot limit (36cm-70cm)."
                << std::endl;
      return true;
    } else if (size < 36.0) {
      std::cout << " -> Dusky Flathead is undersized (< 36cm)." << std::endl;
      return false;
    } else { // size > 70.0
      std::cout << " -> Dusky Flathead is oversized (> 70cm)." << std::endl;
      return false;
    }
  }
  // Flathead (Bluespotted / Tiger - Assuming same rules for Tiger as
  // Bluespotted for simplicity)
  else if (iequals(species, "Bluespotted Flathead") ||
           iequals(species, "Tiger Flathead")) {
    if (size >= 33.0) {
      std::cout
          << " -> Bluespotted/Tiger Flathead meets minimum size (>= 33cm)."
          << std::endl;
      return true;
    } else {
      std::cout << " -> Bluespotted/Tiger Flathead is undersized (< 33cm)."
                << std::endl;
      return false;
    }
  }
  // Luderick
  else if (iequals(species, "Luderick")) {
    if (size >= 27.0) {
      std::cout << " -> Luderick meets minimum size (>= 27cm)." << std::endl;
      return true;
    } else {
      std::cout << " -> Luderick is undersized (< 27cm)." << std::endl;
      return false;
    }
  }
  // Snapper
  else if (iequals(species, "Snapper")) {
    if (size >= 30.0) {
      std::cout << " -> Snapper meets minimum size (>= 30cm)." << std::endl;
      return true;
    } else {
      std::cout << " -> Snapper is undersized (< 30cm)." << std::endl;
      return false;
    }
  }
  // Tailor
  else if (iequals(species, "Tailor")) {
    if (size >= 30.0) {
      std::cout << " -> Tailor meets minimum size (>= 30cm)." << std::endl;
      return true;
    } else {
      std::cout << " -> Tailor is undersized (< 30cm)." << std::endl;
      return false;
    }
  }
  // Mulloway
  else if (iequals(species, "Mulloway")) {
    if (size >= 70.0) {
      std::cout << " -> Mulloway meets minimum size (>= 70cm)." << std::endl;
      return true;
    } else {
      std::cout << " -> Mulloway is undersized (< 70cm)." << std::endl;
      return false;
    }
  }
  // Yellowtail Kingfish
  else if (iequals(species, "Kingfish") ||
           iequals(species, "Yellowtail Kingfish")) {
    if (size >= 65.0) {
      std::cout << " -> Yellowtail Kingfish meets minimum size (>= 65cm)."
                << std::endl;
      return true;
    } else {
      std::cout << " -> Yellowtail Kingfish is undersized (< 65cm)."
                << std::endl;
      return false;
    }
  }
  // Whiting (Sand only specified)
  else if (iequals(species, "Whiting") || iequals(species, "Sand Whiting")) {
    if (size >= 27.0) {
      std::cout << " -> Sand Whiting meets minimum size (>= 27cm)."
                << std::endl;
      return true;
    } else {
      std::cout << " -> Sand Whiting is undersized (< 27cm)." << std::endl;
      return false;
    }
  }
  // ADD MORE 'ELSE IF' BLOCKS HERE FOR OTHER FINFISH SPECIES FROM THE PDF
  // --- ADD TREVALLY RULES HERE ---
  else if (iequals(species, "Silver Trevally")) {
    // Silver Trevally has a minimum size limit of 30cm [cite: 16]
    if (size >= 30.0) {
      std::cout << " -> Silver Trevally meets minimum size (>= 30cm)."
                << std::endl;
      return true;
    } else {
      std::cout << " -> Silver Trevally is undersized (< 30cm)." << std::endl;
      return false;
    }
  } else if (species.find("Trevally") != std::string::npos ||
             species.find("Trevall") != std::string::npos) {
    // Other Trevallies (Giant, Bigeye, etc.) have no size limit listed, only
    // combined bag limit [cite: 16] So, assuming they are individually legal if
    // not Silver Trevally.
    std::cout << " -> Other Trevally species (no size limit specified). "
                 "Assuming OK individually."
              << std::endl;
    return true;
  }
  // --- END TREVALLY RULES ---

  // --- ADD TUNA RULES HERE ---
  else if (iequals(species, "Albacore Tuna") ||
           iequals(species, "Bigeye Tuna") ||
           iequals(species, "Longtail Tuna") ||
           iequals(species, "Yellowfin Tuna")) {
    // No specific size limit mentioned for these in the PDF, only bag limits.
    // Assume individually legal. BagChecker handles the combined limits.
    std::cout << " -> Tuna species (no individual size limit). Assuming OK "
                 "individually."
              << std::endl;
    return true;
  }
  // --- END TUNA RULES --

  // Default case for species not explicitly listed above
  else {
    std::cout
        << " -> Species not found in specific rules. Defaulting to RELEASE."
        << std::endl;
    return false; // Safer default
  }
}