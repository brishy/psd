#ifndef APP_H
#define APP_H

#include <memory> // For std::unique_ptr / std::shared_ptr

// Forward declarations
class SeaPlusPlusEngine;
class SeaCreatureFactory;
struct SeaCreatureData;

class App {
private:
  // Using unique_ptr assuming the App owns the engine and factory
  std::unique_ptr<SeaPlusPlusEngine> engine;
  std::unique_ptr<SeaCreatureFactory> factory;

public:
  App();  // Constructor
  ~App(); // Destructor needed if using unique_ptr with forward declared types

  // Copy/Move semantics (rule of 5 if needed, or disable)
  App(const App &) = delete;
  App &operator=(const App &) = delete;
  App(App &&) = default;            // May need custom definition
  App &operator=(App &&) = default; // May need custom definition

  // Main application loop/entry point
  void runApplication();

private:
  // Helper methods likely needed by runApplication
  SeaCreatureData collectCatchDetails(); // Get input from user
  void displayResult(bool canKeep);      // Show result to user
};

#endif // APP_H