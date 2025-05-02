#ifndef SEACREATURE_H
#define SEACREATURE_H

#include <string>
// Forward declaration is sufficient here if SeaCreatureData is only used
// as a return type or parameter type by pointer/reference in the header.
// However, including it is fine too, especially if used as a member or by
// value.
#include "SeaCreatureData.h"

class SeaCreature {
protected:
  std::string species;
  float size;
  bool hasEggs;

public:
  // Constructor to initialize common properties
  SeaCreature(const std::string &spec, float sz, bool eggs)
      : species(spec), size(sz), hasEggs(eggs) {}

  // Virtual destructor is crucial for base classes with virtual functions
  virtual ~SeaCreature() = default;

  // Pure virtual function makes SeaCreature an abstract class
  virtual std::string getCategory() const = 0;

  // Method to get details (could return a struct or individual values)
  // Returning SeaCreatureData for simplicity based on diagram
  virtual SeaCreatureData getDetails() const {
    SeaCreatureData data;
    data.species = this->species;
    data.size = this->size;
    data.hasEggs = this->hasEggs;
    data.category = this->getCategory(); // Call the virtual function
    // Populate otherRelevantInfo if needed
    return data;
  }

  // Potentially add getters for individual attributes if needed
  const std::string &getSpecies() const { return species; }
  float getSize() const { return size; }
  bool carriesEggs() const { return hasEggs; }
};

#endif // SEACREATURE_H