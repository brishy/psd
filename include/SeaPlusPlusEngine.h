#ifndef SEAPLUSPLUSENGINE_H
#define SEAPLUSPLUSENGINE_H

#include <memory> //unique_ptr

//fwd declarations
class VertebrateChecker;
class InvertebrateChecker;
struct SeaCreatureData;
class App; //optional ref back to App

//mediator class - coordinates between App and Checkers
class SeaPlusPlusEngine {
private:
  //engine owns checkers via unique_ptr
  std::unique_ptr<VertebrateChecker> vertebrateChecker;
  std::unique_ptr<InvertebrateChecker> invertebrateChecker;
  App* app; //raw ptr back to App (doesn't own), maybe unused?

public:
  //ctor - takes ownership of checkers
  SeaPlusPlusEngine(
      std::unique_ptr<VertebrateChecker> vChecker,
      std::unique_ptr<InvertebrateChecker> iChecker);

  //dtor (needed for unique_ptr fwd decl)
  ~SeaPlusPlusEngine();

  //no copy, default move (check needed?)
  SeaPlusPlusEngine(const SeaPlusPlusEngine&) = delete;
  SeaPlusPlusEngine& operator=(const SeaPlusPlusEngine&) = delete;
  SeaPlusPlusEngine(SeaPlusPlusEngine&&) = default; //todo: check default move
  SeaPlusPlusEngine& operator=(SeaPlusPlusEngine&&) = default; //todo: check default move

  //main mediator func - process catch data
  bool processCatch(const SeaCreatureData& creatureData);

  //optional App registration (unused?)
  void registerApp(App* appInstance);
};

#endif