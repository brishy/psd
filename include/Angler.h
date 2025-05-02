#pragma once //include guard

#include "Bag.h" //needs Bag
#include <memory> //unique_ptr
#include <string>
#include <vector>

class App; //fwd declare App

//represents the user interacting with the app
class Angler {
private:
  std::string anglerId;
  Bag anglerBag; //holds the catches

public:
  //ctor
  Angler(const std::string& id) : anglerId(id) {}

  //method for interacting with App (maybe not needed?)
  void interactWithApp(App& application);

  //getters
  const std::string& getId() const { return anglerId; }
  const Bag& getBag() const { return anglerBag; } //getter const
  Bag& getBag() { return anglerBag; }             //getter non-const (for adding stuff)

};