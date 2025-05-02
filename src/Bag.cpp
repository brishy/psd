// src/Bag.cpp
#include "Bag.h"
#include <utility> // For std::move

// Add creature implementation
void Bag::addCreature(std::unique_ptr<SeaCreature> creature) {
  if (creature) { // Check if the pointer is not null
    creatures.push_back(std::move(creature)); // Move ownership into the vector
  }
}

// Count creatures implementation
size_t Bag::creatureCount() const { return creatures.size(); }

// Get all creatures implementation
const std::vector<std::unique_ptr<SeaCreature>> &Bag::getAllCreatures() const {
  return creatures;
}

// Clear implementation
void Bag::clear() { creatures.clear(); }
