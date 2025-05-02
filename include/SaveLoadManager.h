//include/SaveLoadManager.h
#pragma once //include guard

#include <filesystem> //path
#include <optional>   
#include <string>

//fwd declare Angler
class Angler;

//handles saving/loading Angler state (id + bag) to files
class SaveLoadManager {
private:
  const std::filesystem::path saveDirectory = "./saves";
  //private helpers
  bool ensureSaveDirectoryExists() const;
  std::filesystem::path getFilePath(const std::string& anglerId) const;

public:
  //ctor
  SaveLoadManager();

  //save angler state
  bool save(const Angler& angler);
  //load angler state, returns empty optional if failed
  std::optional<Angler> load(const std::string& anglerId);
};