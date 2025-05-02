#include "Bag.h"                  //need Bag
#include "BagChecker.h"           //class under test
#include "InvertebrateChecker.h"  //need checkers for setup
#include "InvertebrateCreature.h" //need creature types
#include "VertebrateChecker.h"    //need checkers for setup
#include "VertebrateCreature.h"   //need creature types
#include "gtest/gtest.h"          //gtest framework
#include <memory>                 //std::unique_ptr
#include <string>
#include <vector>

//check >10 bream/tarwhine fails
TEST(BagCheckerTest, ValidateFailsWhenBreamTarwhineLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 11 valid Bream (>=25cm) - exceeds limit 10
  for (int i = 0; i < 11; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bream", 30.0f, false));
  }
  //should fail, over limit
  ASSERT_FALSE(checker.validate(testBag));
}

//check 10 bream/tarwhine passes
TEST(BagCheckerTest, ValidatePassesWithValidBreamTarwhineCount) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 5 valid Bream (>=25cm)
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bream", 30.0f, false));
  }
  //add 5 valid Tarwhine (>=20cm)
  for (int i = 0; i < 5; ++i) {
    //assume Vert works for Tarwhine here
    testBag.addCreature(std::make_unique<VertebrateCreature>("Tarwhine", 22.0f, false));
  }
  //total 10 = OK
  //assume individuals ok for this test
  ASSERT_TRUE(checker.validate(testBag));
}

//check bag fails if any individual fish is illegal
TEST(BagCheckerTest, ValidateFailsIfIndividualCreatureIsIllegal) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 1 valid Bream
  testBag.addCreature(std::make_unique<VertebrateCreature>("Bream", 30.0f, false));
  //add 1 undersized Bream
  testBag.addCreature(std::make_unique<VertebrateCreature>("Bream", 20.0f, false));
  //count is ok (2), but one fish is illegal
  //should fail (undersized fish)
  ASSERT_FALSE(checker.validate(testBag));
}

//check >5 dusky flathead fails
TEST(BagCheckerTest, ValidateFailsIfDuskyFlatheadLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 6 valid Dusky Flathead (40cm ok) - exceeds limit 5
  for (int i = 0; i < 6; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Dusky Flathead", 40.0f, false));
  }
  //should fail
  ASSERT_FALSE(checker.validate(testBag));
}

//check >10 other flathead fails
TEST(BagCheckerTest, ValidateFailsIfOtherFlatheadLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 11 valid Bluespotted Flathead (>=33cm) - exceeds limit 10
  for (int i = 0; i < 11; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bluespotted Flathead", 35.0f, false));
  }
  //should fail
  ASSERT_FALSE(checker.validate(testBag));
}

//check valid mix of flathead passes
TEST(BagCheckerTest, ValidatePassesWithMixedValidFlathead) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 4 valid Dusky Flathead (limit 5)
  for (int i = 0; i < 4; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Dusky Flathead", 40.0f, false));
  }
  //add 9 valid Bluespotted Flathead (limit 10)
  for (int i = 0; i < 9; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bluespotted Flathead", 35.0f, false));
  }
  //should pass
  ASSERT_TRUE(checker.validate(testBag));
}

//check >10 trevally fails
TEST(BagCheckerTest, ValidateFailsWhenTrevalliesLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 11 valid Silver Trevally (>=30cm)
  //assume Trevally is Vert for test setup
  for (int i = 0; i < 11; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Silver Trevally", 35.0f, false));
  }
  //should fail, over limit 10
  ASSERT_FALSE(checker.validate(testBag));
}

//check 10 trevally passes (mixed types)
TEST(BagCheckerTest, ValidatePassesWithValidTrevalliesCount) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 5 valid Silver Trevally
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Silver Trevally", 35.0f, false));
  }
  //add 5 other Trevally (no size limit)
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bigeye Trevally", 20.0f, false));
  }
  //total 10 = OK
  ASSERT_TRUE(checker.validate(testBag));
}

//check >5 total tuna fails
TEST(BagCheckerTest, ValidateFailsWhenTotalTunaLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 6 small Tuna (<90cm) - over total limit 5
  for (int i = 0; i < 6; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Yellowfin Tuna", 80.0f, false));
  }
  //should fail
  ASSERT_FALSE(checker.validate(testBag));
}

//check >2 large tuna fails
TEST(BagCheckerTest, ValidateFailsWhenLargeTunaLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //add 3 large Tuna (>=90cm) - over large limit 2
  for (int i = 0; i < 3; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Yellowfin Tuna", 100.0f, false));
  }
  //should fail
  ASSERT_FALSE(checker.validate(testBag));
}

//check mixed tuna fails if large limit exceeded
TEST(BagCheckerTest, ValidateFailsWhenLargeTunaLimitExceededMix) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //3 large, 2 small. total=5(ok), large=3(fail)
  for (int i = 0; i < 3; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bigeye Tuna", 100.0f, false));
  }
  for (int i = 0; i < 2; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Albacore Tuna", 80.0f, false));
  }
  //should fail
  ASSERT_FALSE(checker.validate(testBag));
}

//check valid tuna mix passes
TEST(BagCheckerTest, ValidatePassesWithValidTunaMix) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //2 large, 3 small. total=5(ok), large=2(ok)
  for (int i = 0; i < 2; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Longtail Tuna", 95.0f, false));
  }
  for (int i = 0; i < 3; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Yellowfin Tuna", 70.0f, false));
  }
  //should pass
  ASSERT_TRUE(checker.validate(testBag));
}

//check only small tuna ok
TEST(BagCheckerTest, ValidatePassesWithOnlySmallTuna) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //5 small. total=5(ok), large=0(ok)
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Albacore Tuna", 85.0f, false));
  }
  //should pass
  ASSERT_TRUE(checker.validate(testBag));
}

//check only large tuna ok (within limit)
TEST(BagCheckerTest, ValidatePassesWithOnlyLargeTuna) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  //2 large. total=2(ok), large=2(ok)
  for (int i = 0; i < 2; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>("Bigeye Tuna", 110.0f, false));
  }
  //should pass
  ASSERT_TRUE(checker.validate(testBag));
}