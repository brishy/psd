#include "VertebrateChecker.h"
#include "SeaCreature.h" //need full def
#include <algorithm>     //std::equal, std::tolower
#include <iostream>      //cout/cerr
#include <string>

//anon helper namespace for iequals
namespace {
bool iequals(const std::string& a, const std::string& b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a_char, char b_char) {
                      return std::tolower(a_char) == std::tolower(b_char);
                    });
}
} //namespace

//check rules for vertebrates
bool VertebrateChecker::canKeep(const SeaCreature& creature) const {

  const std::string& species = creature.getSpecies();
  float size = creature.getSize();
  //bool hasEggs = creature.carriesEggs(); //hasEggs not usually relevant for verts?

  //--- NSW DPI Rules (Vertebrates) ---

  //Bream/Tarwhine rules
  if (iequals(species, "Bream") || iequals(species, "Black Bream") ||
      iequals(species, "Southern Bream") || iequals(species, "Yellowfin Bream")) {
    return size >= 25.0; //true if >= 25cm
  } else if (iequals(species, "Tarwhine")) {
    return size >= 20.0; //true if >= 20cm
  }
  //Flathead (Dusky) rules
  else if (iequals(species, "Flathead") || iequals(species, "Dusky Flathead")) {
    return size >= 36.0 && size <= 70.0; //true if in slot 36-70cm
  }
  //Flathead (Bluespotted/Tiger) - same rule for both? ok.
  else if (iequals(species, "Bluespotted Flathead") || iequals(species, "Tiger Flathead")) {
    return size >= 33.0; //true if >= 33cm
  }
  //Luderick rules
  else if (iequals(species, "Luderick")) {
    return size >= 27.0; //true if >= 27cm
  }
  //Snapper rules
  else if (iequals(species, "Snapper")) {
    return size >= 30.0; //true if >= 30cm
  }
  //Tailor rules
  else if (iequals(species, "Tailor")) {
    return size >= 30.0; //true if >= 30cm
  }
  //Mulloway rules
  else if (iequals(species, "Mulloway")) {
    return size >= 70.0; //true if >= 70cm
  }
  //Yellowtail Kingfish rules
  else if (iequals(species, "Kingfish") || iequals(species, "Yellowtail Kingfish")) {
    return size >= 65.0; //true if >= 65cm
  }
  //Whiting (Sand) rules - others unspecified?
  else if (iequals(species, "Whiting") || iequals(species, "Sand Whiting")) {
    return size >= 27.0; //true if >= 27cm
  }
  //Trevally rules
  else if (iequals(species, "Silver Trevally")) {
    return size >= 30.0; //Silver Trevally >= 30cm
  } else if (species.find("Trevally") != std::string::npos || species.find("Trevall") != std::string::npos) {
    //Other Trevally - no size limit mentioned in PDF, so assume ok individually.
    return true;
  }
  //Tuna rules
  else if (iequals(species, "Albacore Tuna") ||
           iequals(species, "Bigeye Tuna") ||
           iequals(species, "Longtail Tuna") ||
           iequals(species, "Yellowfin Tuna")) {
    return true;
  }


  //unknown species - default release
  else {
    std::cout << " -> Species '" << species << "' not found in vertebrate rules. Defaulting to RELEASE." << std::endl;
    return false;
  }
}