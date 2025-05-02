// include/BagChecker.h
#ifndef BAGCHECKER_H
#define BAGCHECKER_H

// Forward declarations
class Bag;
class VertebrateChecker;   // <-- Add forward declaration
class InvertebrateChecker; // <-- Add forward declaration

class BagChecker {
private:
  // Store references to the individual checkers
  const VertebrateChecker &v_checker;
  const InvertebrateChecker &i_checker;

public:
  // Constructor takes references to the checkers
  BagChecker(const VertebrateChecker &vc, const InvertebrateChecker &ic);

  bool validate(const Bag &bag) const;

  // Disable copy/move as references make assignment tricky
  BagChecker(const BagChecker &) = delete;
  BagChecker &operator=(const BagChecker &) = delete;
};

#endif // BAGCHECKER_H