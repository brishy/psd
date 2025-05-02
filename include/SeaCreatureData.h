#ifndef SEACREATUREDATA_H
#define SEACREATUREDATA_H

#include <map> // For otherRelevantInfo, adjust if using a different type
#include <string>

// Simple data structure to hold information about a sea creature catch.
// Making members public for easy access, typical for a simple data struct.
struct SeaCreatureData {
  std::string species;
  float size = 0.0f;
  bool hasEggs = false;
  std::string
      category; // e.g., "Vertebrate", "Invertebrate" (can be determined)
  std::map<std::string, std::string> otherRelevantInfo; // Optional extra info
};

#endif // SEACREATUREDATA_H