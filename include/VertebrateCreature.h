#ifndef VERTEBRATECREATURE_H
#define VERTEBRATECREATURE_H

#include "SeaCreature.h"
#include "SeaCreatureData.h" // Needed for constructor parameter
#include <string>

class VertebrateCreature : public SeaCreature {
public:
  // Constructor using SeaCreatureData
  explicit VertebrateCreature(const SeaCreatureData &details)
      : SeaCreature(details.species, details.size, details.hasEggs) {}

  // Constructor using individual parameters
  VertebrateCreature(const std::string &spec, float sz, bool eggs)
      : SeaCreature(spec, sz, eggs) {}

  // Override the pure virtual function from the base class
  std::string getCategory() const override { return "Vertebrate"; }
};

#endif // VERTEBRATECREATURE_H