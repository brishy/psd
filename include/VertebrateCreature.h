#ifndef VERTEBRATECREATURE_H
#define VERTEBRATECREATURE_H

#include "SeaCreature.h"
#include "SeaCreatureData.h" //for ctor
#include <string>

//represents a vertebrate catch (fish)
class VertebrateCreature : public SeaCreature {
public:
  //ctor from data struct
  explicit VertebrateCreature(const SeaCreatureData& details) //explicit to prevent implicit conversion
      : SeaCreature(details.species, details.size, details.hasEggs) {}

  //ctor from params
  VertebrateCreature(const std::string& spec, float sz, bool eggs)
      : SeaCreature(spec, sz, eggs) {}

  //impl base class category method
  std::string getCategory() const override { return "Vertebrate"; }
};

#endif