#ifndef APP_H
#define APP_H

#include <memory> //unique_ptr

//fwd declarations
class SeaPlusPlusEngine;
class SeaCreatureFactory;
struct SeaCreatureData;

//facade class - main interface for user
class App {
private:
  //owns engine and factory via unique_ptr
  std::unique_ptr<SeaPlusPlusEngine> engine;
  std::unique_ptr<SeaCreatureFactory> factory;

public:
  App();  //ctor
  ~App(); //dtor (needed for unique_ptr fwd decl)

  //rule of 5/0 stuff... just disable copy, default move for now
  //todo: check if default move is actually ok later
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  App(App&&) = default;
  App& operator=(App&&) = default;

  //entry point
  void runApplication();

private:
  //internal helpers
  SeaCreatureData collectCatchDetails(); //get user input
  void displayResult(bool canKeep);      //print if keep/release
};

#endif 