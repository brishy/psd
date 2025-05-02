#ifndef VERTEBRATECHECKER_H
#define VERTEBRATECHECKER_H

#include "SeaChecker.h"

// Forward declaration
class SeaCreature;

class VertebrateChecker : public SeaChecker {
public:
  // Override the interface method
  bool canKeep(const SeaCreature &creature) const override;
};

#endif // VERTEBRATECHECKER_H