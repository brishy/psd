#include "Bag.h" //class under test
#include "InvertebrateCreature.h" //need creature types
#include "VertebrateCreature.h"   //need creature types
#include "gtest/gtest.h"          //gtest framework

#include <algorithm> //std::find_if
#include <memory>    //std::unique_ptr
#include <string>
#include <tuple> 
#include <vector>

//test fixture for Bag tests
class BagTest : public ::testing::Test {
protected:
  Bag myBag;
};

//check starts empty, can add
TEST_F(BagTest, StartsEmptyAndCanAddCreature) {
  //check initial count
  ASSERT_EQ(myBag.creatureCount(), 0u); //use 0u for size_t compare warning

  //make a creature
  auto creature = std::make_unique<VertebrateCreature>("Snapper", 35.0, false);

  //add it
  myBag.addCreature(std::move(creature));
  //check count is 1
  ASSERT_EQ(myBag.creatureCount(), 1u);

  //careful comparing unique_ptrs directly
}

//check clear()
TEST_F(BagTest, ClearRemovesAllCreatures) {
  //arrange: add some stuff
  myBag.addCreature(std::make_unique<VertebrateCreature>("Snapper", 35.0, false));
  myBag.addCreature(std::make_unique<InvertebrateCreature>("Lobster", 12.0, false));
  //check setup
  ASSERT_EQ(myBag.creatureCount(), 2u);

  //act: clear
  myBag.clear();
  //assert: should be empty
  ASSERT_EQ(myBag.creatureCount(), 0u);
  ASSERT_TRUE(myBag.getAllCreatures().empty());
}

//check getAll on empty
TEST_F(BagTest, GetAllCreaturesOnEmptyBag) {
  const auto& creatures = myBag.getAllCreatures();
  ASSERT_TRUE(creatures.empty());
  ASSERT_EQ(creatures.size(), 0u);
}

//check adding null doesn't break things
TEST_F(BagTest, AddNullCreatureDoesNotChangeCount) {
  ASSERT_EQ(myBag.creatureCount(), 0u); //start empty

  std::unique_ptr<SeaCreature> null_creature = nullptr;
  //add nullptr
  myBag.addCreature(std::move(null_creature));

  //count still 0
  ASSERT_EQ(myBag.creatureCount(), 0u);
  ASSERT_TRUE(myBag.getAllCreatures().empty());
}

//struct to hold expected creature data for checking getall
struct ExpectedCreatureDetails {
  std::string species;
  float size;
  bool hasEggs;
  std::string category;
  bool found = false; //used to check if we found this one during verification
};

//check getAll returns multiple creatures correctly
TEST_F(BagTest, GetAllCreaturesReturnsMultipleCorrectCreatures) {
  //arrange: setup expected data
  std::vector<ExpectedCreatureDetails> expected_creatures = {
      {"Snapper", 35.5f, false, "Vertebrate"},
      {"Bream", 26.0f, false, "Vertebrate"},
      {"Dusky Flathead", 40.0f, false, "Vertebrate"},
      {"Sand Whiting", 28.0f, false, "Vertebrate"},
      {"Rock Lobster", 11.2f, false, "Invertebrate"}, //no eggs for test
      {"Mud Crab", 9.0f, false, "Invertebrate"},     //no eggs for test
      {"Squid", 15.0f, false, "Invertebrate"}         //size arbitrary, no limit
  };

  //add creatures based on expected data
  for (const auto& details : expected_creatures) {
    if (details.category == "Vertebrate") {
      myBag.addCreature(std::make_unique<VertebrateCreature>(
          details.species, details.size, details.hasEggs));
    } else if (details.category == "Invertebrate") {
      myBag.addCreature(std::make_unique<InvertebrateCreature>(
          details.species, details.size, details.hasEggs));
    }
  }

  //act: get all creatures
  const auto& creatures_in_bag = myBag.getAllCreatures();

  //assert: count matches
  ASSERT_EQ(creatures_in_bag.size(), expected_creatures.size());

  //assert: check details of each creature in bag
  for (const auto& creature_ptr : creatures_in_bag) {
    ASSERT_NE(creature_ptr, nullptr); //check ptr valid

    //find matching expected creature (simple string compare for now)
    auto it = std::find_if(expected_creatures.begin(), expected_creatures.end(),
                           [&](ExpectedCreatureDetails& expected) {
                             return expected.species == creature_ptr->getSpecies() && !expected.found;
                           });

    //check we found a match
    ASSERT_NE(it, expected_creatures.end())
        << "Unexpected creature found in bag: " << creature_ptr->getSpecies();

    if (it != expected_creatures.end()) {
      //verify details
      ASSERT_FLOAT_EQ(creature_ptr->getSize(), it->size);
      ASSERT_EQ(creature_ptr->carriesEggs(), it->hasEggs);
      ASSERT_EQ(creature_ptr->getCategory(), it->category);
      //mark as found
      it->found = true;
    }
  }

  //final check: ensure all expected were found in the bag
  for (const auto& expected : expected_creatures) {
    ASSERT_TRUE(expected.found)
        << "Expected creature not found in bag: " << expected.species;
  }
}