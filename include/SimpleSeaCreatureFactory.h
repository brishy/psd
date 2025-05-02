#ifndef SIMPLESEACREATUREFACTORY_H
#define SIMPLESEACREATUREFACTORY_H

#include "SeaCreatureFactory.h" //implementing sea creature interface

//fwd decls
struct SeaCreatureData;
class SeaCreature;

//simple concrete factory impl
class SimpleSeaCreatureFactory : public SeaCreatureFactory {
public:
  //create the actual creature object based on details
  std::unique_ptr<SeaCreature>
  createSeaCreature(const SeaCreatureData& details) const override;
};

#endif