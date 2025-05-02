#ifndef VERTEBRATECHECKER_H
#define VERTEBRATECHECKER_H

#include "SeaChecker.h" //inherits from this

//fwd decl
class SeaCreature;

//checks rules for vertebrates (fin fish basically)
class VertebrateChecker : public SeaChecker {
public:
  //check rules for this specific creature
  bool canKeep(const SeaCreature& creature) const override;
};

#endif