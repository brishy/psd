// tests/SaveLoadManager_test.cpp
#include "Angler.h"
#include "Bag.h"
#include "SaveLoadManager.h" // Class under test (will create .cpp next)
#include "VertebrateCreature.h"
#include "gtest/gtest.h"
#include <filesystem> // Optional: for directory creation/cleanup (C++17)
#include <fstream>    // For checking if file exists, cleaning up
#include <string>

// Test Fixture for setup/teardown (e.g., manage save files)
class SaveLoadManagerTest : public ::testing::Test {
protected:
  SaveLoadManager slm;
  std::string testAnglerId = "test_angler_save_load";
  std::string testFilePath =
      "./saves/" + testAnglerId + ".txt"; // Match SaveLoadManager format

  void SetUp() override {
    // Ensure save directory exists (create if needed)
    // Requires #include <filesystem>
    try {
      std::filesystem::create_directory("./saves");
    } catch (...) {
    }
    // Clean up any pre-existing save file for this test ID
    std::remove(testFilePath.c_str());
  }

  void TearDown() override {
    // Clean up save file after test
    std::remove(testFilePath.c_str());
    // Optional: remove directory if empty?
    // try { std::filesystem::remove("./saves"); } catch(...) {}
  }
};

TEST_F(SaveLoadManagerTest, SaveAndLoadEmptyAnglerBag) {
  // Arrange: Create an angler with an empty bag
  Angler originalAngler(testAnglerId);
  ASSERT_EQ(originalAngler.getBag().creatureCount(), 0);

  // Act 1: Save the angler
  bool save_success = slm.save(originalAngler);
  ASSERT_TRUE(save_success); // Assert that save reported success

  // Act 2: Load the angler
  std::optional<Angler> loadedAnglerOpt = slm.load(testAnglerId);

  // Assert: Check if loading succeeded and data matches
  ASSERT_TRUE(
      loadedAnglerOpt.has_value()); // Check that load returned an Angler
  if (loadedAnglerOpt) {            // Proceed only if load succeeded
    Angler &loadedAngler = loadedAnglerOpt.value();
    ASSERT_EQ(loadedAngler.getId(), originalAngler.getId());
    ASSERT_EQ(loadedAngler.getBag().creatureCount(), 0); // Check bag is empty
  }
}

TEST_F(SaveLoadManagerTest, SaveAndLoadAnglerWithOneCreature) {
  // Arrange: Create an angler with one creature in the bag
  std::string creatureSpecies = "Snapper";
  float creatureSize = 35.5f;
  bool creatureHasEggs = false;
  std::string creatureCategory = "Vertebrate"; // Important for reconstruction

  Angler originalAngler(testAnglerId); // Use the ID from the fixture
  originalAngler.getBag().addCreature(std::make_unique<VertebrateCreature>(
      creatureSpecies, creatureSize, creatureHasEggs));
  ASSERT_EQ(originalAngler.getBag().creatureCount(), 1); // Verify setup

  // Act 1: Save the angler
  bool save_success = slm.save(originalAngler);
  ASSERT_TRUE(save_success); // Current save might pass this if it returns true

  // Act 2: Load the angler
  std::optional<Angler> loadedAnglerOpt = slm.load(testAnglerId);

  // Assert: Check loading success and data integrity
  ASSERT_TRUE(loadedAnglerOpt.has_value()); // THIS WILL LIKELY FAIL FIRST

  if (loadedAnglerOpt) {
    Angler &loadedAngler = loadedAnglerOpt.value();
    ASSERT_EQ(loadedAngler.getId(), originalAngler.getId());

    // Check the bag contents
    const Bag &loadedBag = loadedAngler.getBag();
    ASSERT_EQ(loadedBag.creatureCount(), 1); // Check if one creature loaded

    if (loadedBag.creatureCount() == 1) {
      const SeaCreature *loadedCreature = loadedBag.getAllCreatures()[0].get();
      ASSERT_NE(loadedCreature, nullptr); // Ensure creature pointer is valid
      if (loadedCreature) {
        ASSERT_EQ(loadedCreature->getSpecies(), creatureSpecies);
        ASSERT_FLOAT_EQ(loadedCreature->getSize(), creatureSize);
        ASSERT_EQ(loadedCreature->carriesEggs(), creatureHasEggs);
        ASSERT_EQ(loadedCreature->getCategory(), creatureCategory);
      }
    }
  }
}