#include "SaveLoadManager.h"
#include "Angler.h"
#include "InvertebrateCreature.h" //needed for load
#include "VertebrateCreature.h"   //needed for load

#include <filesystem> //fs::path, fs::exists, fs::create_directory
#include <fstream>    //ofstream, ifstream
#include <iostream>   //cout, cerr
#include <optional>   //optional<>
#include <sstream>    //stringstream for parsing
#include <stdexcept>  //exceptions for stof
#include <string>     //for string manipulation
#include <vector>     //to use in parsing creature line

//ctor
SaveLoadManager::SaveLoadManager() {
  //make sure ./saves exists on startup
  ensureSaveDirectoryExists();
}

//helper - check/create ./saves dir
bool SaveLoadManager::ensureSaveDirectoryExists() const {
  try {
    if (!std::filesystem::exists(saveDirectory)) {
      std::cout << "Creating save directory: " << saveDirectory.string() << std::endl;
      return std::filesystem::create_directory(saveDirectory);
    }
    return true; //dir exists
  } catch (const std::exception& e) {
    std::cerr << "Error creating/checking save directory: " << e.what() << std::endl;
    return false;
  }
}

//helper - get save file path for ID
//format: ./saves/anglerId.txt
std::filesystem::path SaveLoadManager::getFilePath(const std::string& anglerId) const {
  return saveDirectory / (anglerId + ".txt");
}

//save impl
bool SaveLoadManager::save(const Angler& angler) {
  //make sure dir exists (might be redundant if ctor works, but safe)
  if (!ensureSaveDirectoryExists()) {
    std::cerr << "Error: Save directory cannot be accessed in save()." << std::endl;
    return false;
  }

  std::filesystem::path filePath = getFilePath(angler.getId());
  std::ofstream outputFile(filePath); //open file

  if (!outputFile.is_open()) {
    std::cerr << "Error: Could not open file for saving: " << filePath.string() << std::endl;
    return false; //fail if can't open
  }

  //write ID
  outputFile << "AnglerID:" << angler.getId() << std::endl;
  //write bag section
  outputFile << "BagCreatures:Start" << std::endl;
  const Bag& bag = angler.getBag();
  for (const auto& creature_ptr : bag.getAllCreatures()) {
    if (creature_ptr) { //skip nullptrs
      //write creature line
      //format: Creature:Cat,Spec,Size,Eggs(1/0)
      outputFile << "Creature:"
                 << creature_ptr->getCategory() << ","
                 << creature_ptr->getSpecies() << ","
                 << creature_ptr->getSize() << ","
                 << (creature_ptr->carriesEggs() ? "1" : "0")
                 << std::endl;
    }
  }
  outputFile << "BagCreatures:End" << std::endl;

  //check stream errors before closing
  if (outputFile.fail()) {
    std::cerr << "Error: Failed to write data to file: " << filePath.string() << std::endl;
    outputFile.close();
    return false;
  }

  //close file
  outputFile.close();

  //double check close worked
  if (outputFile.fail() && !outputFile.eof()) {
    std::cerr << "Error: Failed to close file properly after writing: " << filePath.string() << std::endl;
    return false;
  }

  return true;
}

//load impl
std::optional<Angler> SaveLoadManager::load(const std::string& anglerId) {
  std::filesystem::path filePath = getFilePath(anglerId);

  if (!std::filesystem::exists(filePath)) {
    //std::cerr << "Load Info: Save file not found: " << filePath.string() << std::endl; //normal if no save exists
    return std::nullopt;
  }

  std::ifstream inputFile(filePath);
  if (!inputFile.is_open()) {
    std::cerr << "Load Error: Could not open file for loading: " << filePath.string() << std::endl;
    return std::nullopt;
  }

  std::string line;
  std::string loadedAnglerId;
  bool readingBag = false;
  Angler loadedAngler(""); //temp angler obj, ID set later

  //read the file line by line
  int lineNumber = 0;
  while (std::getline(inputFile, line)) {
    lineNumber++;
    if (line.rfind("AnglerID:", 0) == 0) {
      loadedAnglerId = line.substr(9);
      loadedAngler = Angler(loadedAnglerId); //re-make angler now we have ID
    } else if (line == "BagCreatures:Start") {
      readingBag = true;
    } else if (line == "BagCreatures:End") {
      readingBag = false;
    } else if (readingBag && line.rfind("Creature:", 0) == 0) {
      //parse the Creature: line
      std::string data = line.substr(9);
      std::stringstream ss(data);
      std::string segment;
      std::vector<std::string> parts;
      while (std::getline(ss, segment, ',')) { //split by comma
        parts.push_back(segment);
      }

      //expecting 4 parts
      if (parts.size() != 4) {
        std::cerr << "Load WARN: Skipping malformed creature line " << lineNumber << " in " << filePath.string() << std::endl;
        continue; //skip bad line
      }

      std::string category = parts[0];
      std::string species = parts[1];
      float size = 0.0f;
      bool hasEggs = false;

      //parse size (stof)
      try {
        size = std::stof(parts[2]);
      } catch (...) { //lazy catch-all, good enough?
        std::cerr << "Load WARN: Bad size '" << parts[2] << "' on line " << lineNumber << ". Skipping creature." << std::endl;
        continue; //skip if conversion fails
      }

      //parse eggs (1/0)
      if (parts[3] == "1") {
        hasEggs = true;
      } else if (parts[3] != "0") {
        std::cerr << "Load WARN: Weird egg flag '" << parts[3] << "' on line " << lineNumber << ". Assuming false." << std::endl;
        //fall through, hasEggs is already false
      }

      //recreate creature obj from parts
      try {
        std::unique_ptr<SeaCreature> creature;
        if (category == "Vertebrate") {
          creature = std::make_unique<VertebrateCreature>(species, size, hasEggs);
        } else if (category == "Invertebrate") {
          creature = std::make_unique<InvertebrateCreature>(species, size, hasEggs);
        } else {
          std::cerr << "Load WARN: Unknown creature category '" << category << "' on line " << lineNumber << ". Skipping." << std::endl;
        }

        if (creature) {
          //need non-const access to bag here
          loadedAngler.getBag().addCreature(std::move(creature));
        }
      } catch (const std::exception& e) {
        std::cerr << "Load Error: Could not create creature from line " << lineNumber << ": " << e.what() << std::endl;
      }
    } // end parsing creature line
  } //end while getline

  inputFile.close();

  //final checks after reading
  if (loadedAnglerId.empty()) {
    std::cerr << "Load Error: AnglerID not found in file: " << filePath.string() << std::endl;
    return std::nullopt;
  }
  if (loadedAnglerId != anglerId) {
    //weird id mismatch, should fail.
    std::cerr << "Load Error: AnglerID mismatch in file. Expected '" << anglerId
              << "', found '" << loadedAnglerId << "' in " << filePath.string() << std::endl;
    return std::nullopt;
  }

  return loadedAngler; //return loaded angler
}