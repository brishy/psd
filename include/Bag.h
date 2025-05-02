#ifndef BAG_H
#define BAG_H

#include "SeaCreature.h"
#include <memory> //unique_ptr
#include <vector>

//holds the fishes caught. Simple vector of creatures.
class Bag {
private:
  //holds creatures using unique_ptr vector, manages ownership
  std::vector<std::unique_ptr<SeaCreature>> creatures;

public:
  Bag() = default;

  //add creature (moves ownership)
  void addCreature(std::unique_ptr<SeaCreature> creature);

  //how many fish?
  size_t creatureCount() const;

  //get list of creatures (mainly for BagChecker)
  //return const ref, avoid copy
  const std::vector<std::unique_ptr<SeaCreature>>& getAllCreatures() const;

  //empty the bag
  void clear();

  //no copying bags (unique_ptr ownership is messy)
  Bag(const Bag&) = delete;
  Bag& operator=(const Bag&) = delete;
  //move is fine
  Bag(Bag&&) = default;
  Bag& operator=(Bag&&) = default;
};

#endif 