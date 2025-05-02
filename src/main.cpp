// src/main.cpp

#include "App.h" // Include the Facade class header
#include <iostream>

int main() {
  try {
    // Create the application object
    App seaApp;

    // Start the application's main loop/process
    seaApp.runApplication();

  } catch (const std::exception &e) {
    std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    return 1; // Indicate error
  } catch (...) {
    std::cerr << "An unknown error occurred." << std::endl;
    return 1; // Indicate error
  }

  return 0; // Indicate success
}