#ifndef INVERTEBRATECREATURE_H
#define INVERTEBRATECREATURE_H

#include "SeaCreature.h"
#include "SeaCreatureData.h" // Needed for constructor parameter
#include <string>

class InvertebrateCreature : public SeaCreature {
public:
  // Constructor using SeaCreatureData
  explicit InvertebrateCreature(const SeaCreatureData &details)
      : SeaCreature(details.species, details.size, details.hasEggs) {}

  // Constructor using individual parameters
  InvertebrateCreature(const std::string &spec, float sz, bool eggs)
      : SeaCreature(spec, sz, eggs) {}

  // Override the pure virtual function from the base class
  std::string getCategory() const override { return "Invertebrate"; }
};

#endif // INVERTEBRATECREATURE_H