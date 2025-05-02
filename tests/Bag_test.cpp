// tests/Bag_test.cpp
#include "Bag.h" // Include the header for the class we are testing
#include "VertebrateCreature.h" // Needed to create a creature for testing
#include "gtest/gtest.h"        // Include Google Test framework

// --- FIX: Include header for InvertebrateCreature ---
#include "InvertebrateCreature.h"
// --- End FIX ---

#include <algorithm> // For std::find_if
#include <memory>    // For std::unique_ptr
#include <string>    // Include string
#include <tuple>     // Include tuple for storing expected data
#include <vector>    // Include vector for storing expected data

// Test fixture (optional, but good practice for setup/teardown)
class BagTest : public ::testing::Test {
protected:
  Bag myBag; // Each test gets a fresh Bag instance
};

// Test case: Check if the bag is initially empty and can add a creature
TEST_F(BagTest, StartsEmptyAndCanAddCreature) {
  // ASSERT_EQ checks if two values are equal.
  ASSERT_EQ(myBag.creatureCount(), 0); // Test initial state

  // Create a sample creature to add (using existing classes)
  auto creature = std::make_unique<VertebrateCreature>("Snapper", 35.0, false);

  // --- FIX: Comment out or remove unused variable to silence warning ---
  // SeaCreature* raw_ptr = creature.get(); // Not currently used
  // --- End FIX ---

  // Add the creature
  myBag.addCreature(std::move(creature));

  // Check state after adding
  ASSERT_EQ(myBag.creatureCount(), 1);

  // Optional: Check if we can retrieve the added creature
  // ASSERT_EQ(myBag.getAllCreatures().size(), 1);
  // Need to be careful comparing unique_ptrs directly, compare contained object
  // if needed ASSERT_EQ(myBag.getAllCreatures()[0]->getSpecies(), "Snapper");
}

// Test case: Check if clear() removes creatures
TEST_F(BagTest, ClearRemovesAllCreatures) {
  // Arrange: Add some creatures first
  myBag.addCreature(
      std::make_unique<VertebrateCreature>("Snapper", 35.0, false));
  // Now this line should work as InvertebrateCreature is known
  myBag.addCreature(
      std::make_unique<InvertebrateCreature>("Lobster", 12.0, false));
  ASSERT_EQ(myBag.creatureCount(), 2); // Verify setup

  // Act: Call the method under test
  myBag.clear();

  // Assert: Check if the bag is now empty
  ASSERT_EQ(myBag.creatureCount(), 0);
  ASSERT_TRUE(myBag.getAllCreatures().empty()); // Also check vector directly
}

// Add more TEST_F blocks for other Bag functionality later...

TEST_F(BagTest, GetAllCreaturesOnEmptyBag) {
  const auto &creatures = myBag.getAllCreatures();
  ASSERT_TRUE(creatures.empty());
  ASSERT_EQ(creatures.size(), 0);
}

TEST_F(BagTest, AddNullCreatureDoesNotChangeCount) {
  ASSERT_EQ(myBag.creatureCount(), 0); // Start empty

  std::unique_ptr<SeaCreature> null_creature = nullptr;
  myBag.addCreature(std::move(null_creature)); // Try adding nullptr

  ASSERT_EQ(myBag.creatureCount(), 0);          // Count should remain 0
  ASSERT_TRUE(myBag.getAllCreatures().empty()); // Bag should still be empty
}

// Define a structure to hold expected details for comparison
struct ExpectedCreatureDetails {
  std::string species;
  float size;
  bool hasEggs;
  std::string category;
  bool found = false; // Flag to track if this creature was found in the bag
};

TEST_F(BagTest, GetAllCreaturesReturnsMultipleCorrectCreatures) {
  // Arrange: Define details for a variety of creatures to add
  // Use a vector to store the expected details for easier verification later
  std::vector<ExpectedCreatureDetails> expected_creatures = {
      {"Snapper", 35.5f, false, "Vertebrate"},        // [cite: 15]
      {"Bream", 26.0f, false, "Vertebrate"},          // [cite: 12]
      {"Dusky Flathead", 40.0f, false, "Vertebrate"}, // [cite: 13]
      {"Sand Whiting", 28.0f, false, "Vertebrate"},   // [cite: 16]
      {"Rock Lobster", 11.2f, false,
       "Invertebrate"}, // [cite: 20] (No eggs for test)
      {"Mud Crab", 9.0f, false,
       "Invertebrate"}, // [cite: 19] (No eggs for test)
      {"Squid", 15.0f, false,
       "Invertebrate"} // [cite: 20] (Size arbitrary, no limit)
  };

  // Add creatures to the bag
  for (const auto &details : expected_creatures) {
    if (details.category == "Vertebrate") {
      myBag.addCreature(std::make_unique<VertebrateCreature>(
          details.species, details.size, details.hasEggs));
    } else if (details.category == "Invertebrate") {
      myBag.addCreature(std::make_unique<InvertebrateCreature>(
          details.species, details.size, details.hasEggs));
    }
  }

  // Act: Get the creatures from the bag
  const auto &creatures_in_bag = myBag.getAllCreatures();

  // Assert: Check count first
  ASSERT_EQ(creatures_in_bag.size(), expected_creatures.size());

  // Assert: Check details of each creature found in the bag
  for (const auto &creature_ptr : creatures_in_bag) {
    ASSERT_NE(creature_ptr, nullptr); // Ensure pointer is not null

    // Find the matching expected creature based on species name
    // (case-insensitive)
    auto it =
        std::find_if(expected_creatures.begin(), expected_creatures.end(),
                     [&](ExpectedCreatureDetails &expected) {
                       // Use the iequals helper if you have it accessible here,
                       // otherwise basic compare For robustness, let's assume
                       // basic compare first (adjust if needed)
                       return expected.species == creature_ptr->getSpecies() &&
                              !expected.found;
                     });

    // Check if a matching expected creature was found
    ASSERT_NE(it, expected_creatures.end())
        << "Unexpected creature found in bag: " << creature_ptr->getSpecies();

    if (it != expected_creatures.end()) {
      // Verify details against the matched expected creature
      ASSERT_FLOAT_EQ(creature_ptr->getSize(), it->size);
      ASSERT_EQ(creature_ptr->carriesEggs(), it->hasEggs);
      ASSERT_EQ(creature_ptr->getCategory(), it->category);
      it->found = true; // Mark this expected creature as found
    }
  }

  // Final Check: Ensure ALL expected creatures were found
  for (const auto &expected : expected_creatures) {
    ASSERT_TRUE(expected.found)
        << "Expected creature not found in bag: " << expected.species;
  }
}