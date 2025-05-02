#ifndef BAGCHECKER_H
#define BAGCHECKER_H

//fwd declarations
class Bag;
class VertebrateChecker; //fwd decl
class InvertebrateChecker; //fwd decl

//checks aggregate bag limits, uses individual checkers first
class BagChecker {
private:
  //needs refs to individual checkers (passed in ctor)
  const VertebrateChecker& v_checker;
  const InvertebrateChecker& i_checker;

public:
  //ctor - takes checker refs
  BagChecker(const VertebrateChecker& vc, const InvertebrateChecker& ic);

  //main validation logic for the whole bag
  bool validate(const Bag& bag) const;

  //no copy/move (refs make this annoying)
  BagChecker(const BagChecker&) = delete;
  BagChecker& operator=(const BagChecker&) = delete;
};

#endif 