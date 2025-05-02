// include/SaveLoadManager.h
#pragma once // IMPORTANT: Include guard

#include <filesystem> // Needed for path
#include <optional>   // Needed for the return type of load
#include <string>

// Forward declare Angler if only pointers/references are used in the header
class Angler;
// If Angler object itself or its full methods needed, #include "Angler.h"

class SaveLoadManager {
private:
  const std::filesystem::path saveDirectory =
      "./saves"; // Or initialize in constructor
  // Helper methods declared here
  bool ensureSaveDirectoryExists() const;
  std::filesystem::path getFilePath(const std::string &anglerId) const;

public:
  SaveLoadManager(); // Constructor declaration

  // Method declarations MUST match the definitions in the .cpp
  bool save(const Angler &angler);
  std::optional<Angler> load(const std::string &anglerId);
};