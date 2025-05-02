#ifndef SEACREATUREFACTORY_H
#define SEACREATUREFACTORY_H

#include <memory> //unique_ptr

//fwd declarations
struct SeaCreatureData;
class SeaCreature;

//factory interface for making creatures
class SeaCreatureFactory {
public:
  //virtual dtor
  virtual ~SeaCreatureFactory() = default;

  //pure virtual - create creature based on details
  //returns unique_ptr (manages ownership)
  virtual std::unique_ptr<SeaCreature>
  createSeaCreature(const SeaCreatureData& details) const = 0;
};

#endif