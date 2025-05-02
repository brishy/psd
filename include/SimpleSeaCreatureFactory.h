#ifndef SIMPLESEACREATUREFACTORY_H
#define SIMPLESEACREATUREFACTORY_H

#include "SeaCreatureFactory.h"

// Forward declarations (already in SeaCreatureFactory.h, but good practice)
struct SeaCreatureData;
class SeaCreature;

// A simple concrete factory implementing the SeaCreatureFactory interface
class SimpleSeaCreatureFactory : public SeaCreatureFactory {
public:
  // Override the interface method
  std::unique_ptr<SeaCreature>
  createSeaCreature(const SeaCreatureData &details) const override;
};

#endif // SIMPLESEACREATUREFACTORY_H