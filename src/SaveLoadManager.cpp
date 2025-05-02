// src/SaveLoadManager.cpp
#include "SaveLoadManager.h"
#include "Angler.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept> // For potential exceptions during parsing/creation
#include <string>
#include <vector> // Needed for parsing helper maybe

// --- ADD THESE INCLUDES IF NOT ALREADY PRESENT ---
#include "InvertebrateCreature.h"
#include "VertebrateCreature.h"

// Constructor Implementation
SaveLoadManager::SaveLoadManager() {
  // Ensure the directory exists when the manager is created.
  ensureSaveDirectoryExists();
}

// Helper to ensure the save directory exists
bool SaveLoadManager::ensureSaveDirectoryExists() const {
  // 'saveDirectory' is a private member variable defined in SaveLoadManager.h
  try {
    if (!std::filesystem::exists(saveDirectory)) {
      // Use .string() or appropriate method if path object needs conversion for
      // output
      std::cout << "Creating save directory: " << saveDirectory.string()
                << std::endl;
      return std::filesystem::create_directory(saveDirectory);
    }
    return true; // Already exists
  } catch (const std::exception &e) {
    std::cerr << "Error creating/checking save directory: " << e.what()
              << std::endl;
    return false;
  }
}

// Helper to get the full file path for an angler ID
std::filesystem::path
SaveLoadManager::getFilePath(const std::string &anglerId) const {
  // 'saveDirectory' is a private member variable defined in SaveLoadManager.h
  // Simple format: ID.txt in the save directory
  return saveDirectory / (anglerId + ".txt");
}

bool SaveLoadManager::save(const Angler &angler) {
  if (!ensureSaveDirectoryExists()) {
    std::cerr << "Error: Save directory cannot be accessed." << std::endl;
    return false;
  }

  std::filesystem::path filePath = getFilePath(angler.getId());
  std::ofstream outputFile(filePath); // Open file for writing

  if (!outputFile.is_open()) {
    std::cerr << "Error: Could not open file for saving: " << filePath
              << std::endl;
    return false; // Could not open the file
  }

  // --- Write Angler ID ---
  outputFile << "AnglerID:" << angler.getId() << std::endl;

  // --- Write Bag Creatures ---
  outputFile << "BagCreatures:Start" << std::endl;
  const Bag &bag = angler.getBag(); // Get the bag
  for (const auto &creature_ptr :
       bag.getAllCreatures()) { // Loop through creatures
    if (creature_ptr) {         // Check if pointer is valid
      // Write creature details on one line, comma-separated
      // Format: Creature:<Category>,<Species>,<Size>,<HasEggs>
      outputFile << "Creature:" << creature_ptr->getCategory()
                 << "," // Vertebrate/Invertebrate
                 << creature_ptr->getSpecies() << "," << creature_ptr->getSize()
                 << ","
                 << (creature_ptr->carriesEggs() ? "1"
                                                 : "0") // Save bool as 1 or 0
                 << std::endl;
    }
  }
  outputFile << "BagCreatures:End" << std::endl;

  // --- Check for Write Errors ---
  if (outputFile.fail()) {
    std::cerr << "Error: Failed to write data to file: " << filePath
              << std::endl;
    outputFile.close(); // Attempt to close before returning
    return false;
  }

  outputFile.close(); // Close the file explicitly

  // Check if the file was closed successfully (optional, but good practice)
  if (outputFile.fail() && !outputFile.eof()) {
    std::cerr << "Error: Failed to close file properly after writing: "
              << filePath << std::endl;
    return false;
  }

  std::cout << "SaveLoadManager::save completed successfully for "
            << angler.getId() << std::endl;
  return true; // Report success
}

std::optional<Angler> SaveLoadManager::load(const std::string &anglerId) {
  std::filesystem::path filePath = getFilePath(anglerId);

  if (!std::filesystem::exists(filePath)) { /* ... error handling ... */
    return std::nullopt;
  }

  std::ifstream inputFile(filePath);
  if (!inputFile.is_open()) { /* ... error handling ... */
    return std::nullopt;
  }

  std::string line;
  std::string loadedAnglerId;
  bool readingBag = false;
  Angler loadedAngler(""); // Create temporary Angler - ID will be set later

  // --- Read and Parse File ---
  int lineNumber = 0; // For error messages
  while (std::getline(inputFile, line)) {
    lineNumber++;
    if (line.rfind("AnglerID:", 0) == 0) {
      loadedAnglerId = line.substr(9);
      // Re-create angler here once ID is known (or set ID later)
      loadedAngler = Angler(loadedAnglerId); // Re-assign now we have the ID
    } else if (line == "BagCreatures:Start") {
      readingBag = true;
    } else if (line == "BagCreatures:End") {
      readingBag = false;
    } else if (readingBag && line.rfind("Creature:", 0) == 0) {
      // --- Parse Creature Line ---
      std::string data = line.substr(9); // Get data after "Creature:"
      std::stringstream ss(data);
      std::string segment;
      std::vector<std::string> parts;

      while (std::getline(ss, segment, ',')) { // Split by comma
        parts.push_back(segment);
      }

      if (parts.size() != 4) {
        std::cerr << "Load Error: Malformed creature data on line "
                  << lineNumber << " in " << filePath.string() << std::endl;
        continue; // Skip this creature line
      }

      // Extract parts
      std::string category = parts[0];
      std::string species = parts[1];
      float size = 0.0f;
      bool hasEggs = false;

      // Safely convert size (handle potential errors)
      try {
        size = std::stof(parts[2]);
      } catch (const std::invalid_argument &ia) {
        std::cerr << "Load Error: Invalid size format '" << parts[2]
                  << "' on line " << lineNumber << std::endl;
        continue;
      } catch (const std::out_of_range &oor) {
        std::cerr << "Load Error: Size out of range '" << parts[2]
                  << "' on line " << lineNumber << std::endl;
        continue;
      }

      // Safely convert hasEggs (expecting "1" or "0")
      if (parts[3] == "1") {
        hasEggs = true;
      } else if (parts[3] == "0") {
        hasEggs = false;
      } else {
        std::cerr << "Load Error: Invalid egg flag '" << parts[3]
                  << "' on line " << lineNumber << std::endl;
        continue; // Skip if invalid flag
      }

      // --- Create and Add Creature ---
      try {
        std::unique_ptr<SeaCreature> creature;
        if (category == "Vertebrate") {
          creature =
              std::make_unique<VertebrateCreature>(species, size, hasEggs);
        } else if (category == "Invertebrate") {
          creature =
              std::make_unique<InvertebrateCreature>(species, size, hasEggs);
        } else {
          std::cerr << "Load Warning: Unknown creature category '" << category
                    << "' on line " << lineNumber << ". Skipping." << std::endl;
        }

        if (creature) {
          loadedAngler.getBag().addCreature(std::move(creature));
        }
      } catch (const std::exception &e) {
        std::cerr << "Load Error: Could not create creature from line "
                  << lineNumber << ": " << e.what() << std::endl;
        // Decide whether to continue or fail loading entirely
      }
    }
  } // End while getline

  inputFile.close();

  // --- Final Validation ---
  if (loadedAnglerId.empty()) { /* ... error handling ... */
    return std::nullopt;
  }
  if (loadedAnglerId != anglerId) { /* ... error handling ... */
    return std::nullopt;
  }

  std::cout << "SaveLoadManager::load completed successfully for " << anglerId
            << std::endl;
  return loadedAngler; // Return the loaded angler (potentially with creatures)
}