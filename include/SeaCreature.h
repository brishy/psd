#ifndef SEACREATURE_H
#define SEACREATURE_H

#include <string>
#include "SeaCreatureData.h" //using the data struct here

//base class for things you catch
class SeaCreature {
protected:
  //common data for all creatures
  std::string species;
  float size;
  bool hasEggs;

public:
  //base ctor
  SeaCreature(const std::string& spec, float sz, bool eggs)
      : species(spec), size(sz), hasEggs(eggs) {}

  //virtual dtor needed for inheritance
  virtual ~SeaCreature() = default;

  //pure virtual - subclasses must say what category they are
  virtual std::string getCategory() const = 0;

  //get all details as a struct
  virtual SeaCreatureData getDetails() const {
    SeaCreatureData data;
    data.species = this->species;
    data.size = this->size;
    data.hasEggs = this->hasEggs;
    data.category = this->getCategory(); //important: calls virtual method
    //data.otherRelevantInfo = ... //not used yet
    return data;
  }

  //basic getters
  const std::string& getSpecies() const { return species; }
  float getSize() const { return size; }
  bool carriesEggs() const { return hasEggs; }
};

#endif 