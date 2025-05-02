#include "Bag.h"
#include <utility> //to std::move

//addCreature impl
void Bag::addCreature(std::unique_ptr<SeaCreature> creature) {
  if (creature) { //not adding nullptrs
    creatures.push_back(std::move(creature)); //to move ownership into vector
  }
}

//creatureCount impl
size_t Bag::creatureCount() const {
  return creatures.size();
}

//getAllCreatures impl
const std::vector<std::unique_ptr<SeaCreature>>& Bag::getAllCreatures() const {
  return creatures;
}

//clear impl
void Bag::clear() {
  creatures.clear(); //vector clear handles unique_ptr deletion
}