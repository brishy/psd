#ifndef SEACHECKER_H
#define SEACHECKER_H

//fwd decl
class SeaCreature;

//checker interface (base class)
class SeaChecker {
public:
  //virtual dtor for base class
  virtual ~SeaChecker() = default;

  //pure virtual func - subclasses must implement canKeep
  virtual bool canKeep(const SeaCreature& creature) const = 0;
};

#endif 