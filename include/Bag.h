// include/Bag.h
#ifndef BAG_H
#define BAG_H

#include "SeaCreature.h" // Base class for creatures
#include <memory>        // For std::unique_ptr
#include <vector>

class Bag {
private:
  // Use a vector of unique_ptr to hold creatures and manage ownership
  std::vector<std::unique_ptr<SeaCreature>> creatures;

public:
  Bag() = default; // Default constructor

  // Method to add a creature (takes ownership)
  void addCreature(std::unique_ptr<SeaCreature> creature);

  // Method to count creatures
  size_t creatureCount() const;

  // Method to get all creatures (e.g., for BagChecker)
  // Returning a const reference to avoid copying the vector
  const std::vector<std::unique_ptr<SeaCreature>> &getAllCreatures() const;

  // Method to clear the bag
  void clear();

  // Disable copy semantics for simplicity if Bag owns unique_ptrs
  Bag(const Bag &) = delete;
  Bag &operator=(const Bag &) = delete;
  // Allow move semantics (optional, but good practice)
  Bag(Bag &&) = default;
  Bag &operator=(Bag &&) = default;
};

#endif // BAG_H