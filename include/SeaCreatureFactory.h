#ifndef SEACREATUREFACTORY_H
#define SEACREATUREFACTORY_H

#include <memory> // For std::unique_ptr or std::shared_ptr

// Forward declarations
struct SeaCreatureData;
class SeaCreature;

// Interface (Abstract Class) for creating SeaCreature instances
class SeaCreatureFactory {
public:
  virtual ~SeaCreatureFactory() = default; // Virtual destructor

  // Pure virtual function to create a SeaCreature based on details.
  // Returning a smart pointer (e.g., unique_ptr) is good practice for
  // ownership.
  virtual std::unique_ptr<SeaCreature>
  createSeaCreature(const SeaCreatureData &details) const = 0;
};

#endif // SEACREATUREFACTORY_H