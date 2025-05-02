#ifndef INVERTEBRATECHECKER_H
#define INVERTEBRATECHECKER_H

#include "SeaChecker.h"

//fwd decl
class SeaCreature;

//checks rules for invertebrates (crabs, lobsters, etc)
class InvertebrateChecker : public SeaChecker {
public:
  //checks rules for this specific creature
  bool canKeep(const SeaCreature& creature) const override;
};

#endif 