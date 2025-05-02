#include "SaveLoadManager.h" //class under test
#include "Angler.h"
#include "Bag.h"
#include "VertebrateCreature.h" //need for creating test creature
#include "gtest/gtest.h"
#include <filesystem> //for file cleanup
#include <fstream>    //remove()
#include <string>
#include <optional>   //optional<>

//test fixture, manages save file setup/teardown
class SaveLoadManagerTest : public ::testing::Test {
protected:
  SaveLoadManager slm;
  std::string testAnglerId = "test_angler_save_load";
  std::string testFilePath = "./saves/" + testAnglerId + ".txt";

  void SetUp() override {
    //ensure ./saves exists for test output
    try {
      std::filesystem::create_directory("./saves");
    } catch (...) { //ignore if exists or error
    }
    //delete test save file before test run
    std::remove(testFilePath.c_str());
  }

  void TearDown() override {
    //delete test save file after test run
    std::remove(testFilePath.c_str());
    //TODO: maybe remove ./saves dir after all tests? if empty?
  }
};

//check saving/loading with empty bag works
TEST_F(SaveLoadManagerTest, SaveAndLoadEmptyAnglerBag) {
  //arrange: angler w/ empty bag
  Angler originalAngler(testAnglerId);
  ASSERT_EQ(originalAngler.getBag().creatureCount(), 0u);

  //act 1: save
  bool save_success = slm.save(originalAngler);
  ASSERT_TRUE(save_success); //check save ok

  //act 2: load
  std::optional<Angler> loadedAnglerOpt = slm.load(testAnglerId);

  //assert: check load ok & data matches
  ASSERT_TRUE(loadedAnglerOpt.has_value()); //check optional has value
  if (loadedAnglerOpt) { //only check details if load returned something
    Angler& loadedAngler = loadedAnglerOpt.value();
    ASSERT_EQ(loadedAngler.getId(), originalAngler.getId());
    //check loaded bag empty
    ASSERT_EQ(loadedAngler.getBag().creatureCount(), 0u);
  }
}

//check saving/loading with one creature works
TEST_F(SaveLoadManagerTest, SaveAndLoadAnglerWithOneCreature) {
  //arrange: angler w/ 1 creature
  std::string creatureSpecies = "Snapper";
  float creatureSize = 35.5f;
  bool creatureHasEggs = false;
  std::string creatureCategory = "Vertebrate"; //needed for load reconstruction

  Angler originalAngler(testAnglerId);
  originalAngler.getBag().addCreature(
      std::make_unique<VertebrateCreature>(creatureSpecies, creatureSize, creatureHasEggs));
  //check arrange step ok
  ASSERT_EQ(originalAngler.getBag().creatureCount(), 1u);

  //act 1: save
  bool save_success = slm.save(originalAngler);
  ASSERT_TRUE(save_success);

  //act 2: load
  std::optional<Angler> loadedAnglerOpt = slm.load(testAnglerId);

  //assert: check load ok
  ASSERT_TRUE(loadedAnglerOpt.has_value());

  if (loadedAnglerOpt) {
    Angler& loadedAngler = loadedAnglerOpt.value();
    //check ID
    ASSERT_EQ(loadedAngler.getId(), originalAngler.getId());

    //check bag contents
    const Bag& loadedBag = loadedAngler.getBag();
    ASSERT_EQ(loadedBag.creatureCount(), 1u); //check bag has 1 creature

    if (loadedBag.creatureCount() == 1u) { //use 1u for size_t compare
      const SeaCreature* loadedCreature = loadedBag.getAllCreatures()[0].get();
      ASSERT_NE(loadedCreature, nullptr); //check loaded ptr ok
      if (loadedCreature) {
        //check creature details match
        ASSERT_EQ(loadedCreature->getSpecies(), creatureSpecies);
        ASSERT_FLOAT_EQ(loadedCreature->getSize(), creatureSize);
        ASSERT_EQ(loadedCreature->carriesEggs(), creatureHasEggs);
        ASSERT_EQ(loadedCreature->getCategory(), creatureCategory);
      }
    }
  }
}

