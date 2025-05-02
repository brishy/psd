// include/Angler.h
#pragma once // Good practice: include guard

#include "Bag.h"  // Assuming Angler has a Bag
#include <memory> // For unique_ptr
#include <string>
#include <vector>

class App; // <--- ADD THIS FORWARD DECLARATION

class Angler {
private:
  std::string anglerId;
  Bag anglerBag; // Assuming Angler has a Bag object

public:
  // Constructor(s) - Assuming you have one like this
  Angler(const std::string &id) : anglerId(id) {}

  // --- ADD THE MISSING DECLARATION HERE ---
  void interactWithApp(App &application); // <--- ADD THIS DECLARATION

  // --- Other Angler methods ---
  const std::string &getId() const { return anglerId; }
  const Bag &getBag() const { return anglerBag; } // Method to get const ref
  Bag &getBag() { return anglerBag; } // Method to get non-const ref (if needed)

  // Potentially add methods to modify the bag if needed outside save/load
  // void addCreatureToBag(std::unique_ptr<SeaCreature> creature) {
  //     anglerBag.addCreature(std::move(creature));
  // }
};