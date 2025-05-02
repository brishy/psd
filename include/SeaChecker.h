#ifndef SEACHECKER_H
#define SEACHECKER_H

// Forward declaration can work if SeaCreature is only used via
// pointer/reference
class SeaCreature;
// Or include if needed directly: #include "SeaCreature.h"

// Interface (Abstract Class) for checking sea creatures against regulations
class SeaChecker {
public:
  virtual ~SeaChecker() = default; // Virtual destructor for interface

  // Pure virtual function to check if a creature can be kept
  virtual bool canKeep(const SeaCreature &creature) const = 0;
};

#endif // SEACHECKER_H