#include "App.h" //facade
#include <iostream> //cerr, cout

int main() {
  try {
    //making the app
    App seaApp;
    //running it
    seaApp.runApplication();
  } catch (const std::exception& e) {
    //catch for exceptions
    std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    return 1; //error 
  } catch (...) {
    //catch anything else
    std::cerr << "An unknown error occurred." << std::endl;
    return 1; //error
  }

  return 0; //success
}