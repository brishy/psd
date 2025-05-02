#ifndef INVERTEBRATECREATURE_H
#define INVERTEBRATECREATURE_H

#include "SeaCreature.h"
#include "SeaCreatureData.h" //for ctor

#include <string>

//represents an invertebrate catch
class InvertebrateCreature : public SeaCreature {
public:
  //ctor from data struct
  explicit InvertebrateCreature(const SeaCreatureData& details) //explicit to prevent implicit conversion
      : SeaCreature(details.species, details.size, details.hasEggs) {}

  //ctor from params
  InvertebrateCreature(const std::string& spec, float sz, bool eggs)
      : SeaCreature(spec, sz, eggs) {}

  //impl base class category method
  std::string getCategory() const override { return "Invertebrate"; }
};

#endif 