#ifndef INVERTEBRATECHECKER_H
#define INVERTEBRATECHECKER_H

#include "SeaChecker.h"

// Forward declaration
class SeaCreature;

class InvertebrateChecker : public SeaChecker {
public:
  // Override the interface method
  bool canKeep(const SeaCreature &creature) const override;
};

#endif // INVERTEBRATECHECKER_H