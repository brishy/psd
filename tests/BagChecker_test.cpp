// tests/BagChecker_test.cpp
#include "Bag.h"        // Used to create test bags
#include "BagChecker.h" // Class under test (will create)
#include "InvertebrateChecker.h"
#include "InvertebrateCreature.h" // Need concrete creatures for testing
#include "VertebrateChecker.h" // Need the individual checkers for testing setup
#include "VertebrateCreature.h" // Need concrete creatures for testing
#include "gtest/gtest.h"        // Google Test
#include <memory>               // For std::unique_ptr
#include <string>               // For species names
#include <vector>               // Potentially useful

TEST(BagCheckerTest, ValidateFailsWhenBreamTarwhineLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker); // Pass checkers to constructor
  Bag testBag;

  // Add 11 valid Bream (size >= 25cm) - Exceeds limit of 10
  for (int i = 0; i < 11; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Bream", 30.0f, false));
  }

  // Expect validate to return false because the combined limit is broken
  ASSERT_FALSE(checker.validate(testBag)); // This test should FAIL initially
}

TEST(BagCheckerTest, ValidatePassesWithValidBreamTarwhineCount) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker); // Pass checkers to constructor
  Bag testBag;

  // Add 5 valid Bream (size >= 25cm)
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Bream", 30.0f, false));
  }
  // Add 5 valid Tarwhine (size >= 20cm)
  for (int i = 0; i < 5; ++i) {
    // Assuming VertebrateCreature works for Tarwhine for this test setup
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Tarwhine", 22.0f, false));
  }

  // Combined total is 10, which is allowed.
  // Assume individual checks pass for this test's focus.
  ASSERT_TRUE(checker.validate(
      testBag)); // This test should also FAIL initially (because default impl
                 // might be false, or true which makes the *other* test pass
                 // wrongly)
}

TEST(BagCheckerTest, ValidateFailsIfIndividualCreatureIsIllegal) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker); // Pass checkers to constructor
  Bag testBag;

  // Add one valid Bream (size >= 25cm)
  testBag.addCreature(
      std::make_unique<VertebrateCreature>("Bream", 30.0f, false));
  // Add one *undersized* Bream (size < 25cm)
  testBag.addCreature(
      std::make_unique<VertebrateCreature>("Bream", 20.0f, false));

  // Combined Bream count (2) is okay, but the second fish is illegal
  // individually.
  ASSERT_FALSE(
      checker.validate(testBag)); // Should fail because of the undersized Bream
}

TEST(BagCheckerTest, ValidateFailsIfDuskyFlatheadLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 6 valid Dusky Flathead (size 40cm) - Exceeds limit of 5
  for (int i = 0; i < 6; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Dusky Flathead", 40.0f, false));
  }
  ASSERT_FALSE(checker.validate(testBag)); // Should fail
}

TEST(BagCheckerTest, ValidateFailsIfOtherFlatheadLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 11 valid Bluespotted Flathead (size >= 33cm) - Exceeds limit of 10
  for (int i = 0; i < 11; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>(
        "Bluespotted Flathead", 35.0f, false));
  }
  ASSERT_FALSE(checker.validate(testBag)); // Should fail
}

TEST(BagCheckerTest, ValidatePassesWithMixedValidFlathead) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 4 valid Dusky Flathead (size 40cm) - Limit 5
  for (int i = 0; i < 4; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Dusky Flathead", 40.0f, false));
  }
  // Add 9 valid Bluespotted Flathead (size >= 33cm) - Limit 10
  for (int i = 0; i < 9; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>(
        "Bluespotted Flathead", 35.0f, false));
  }
  ASSERT_TRUE(checker.validate(testBag)); // Should pass
}

TEST(BagCheckerTest, ValidateFailsWhenTrevalliesLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 11 valid Silver Trevally (size >= 30cm is needed for Silver)
  for (int i = 0; i < 11; ++i) {
    // Assuming all Trevallies are Vertebrates for testing class structure
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Silver Trevally", 35.0f, false));
  }
  ASSERT_FALSE(checker.validate(testBag)); // Should fail (over limit 10)
}

TEST(BagCheckerTest, ValidatePassesWithValidTrevalliesCount) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 5 valid Silver Trevally
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Silver Trevally", 35.0f, false));
  }
  // Add 5 other Trevally (e.g., Bigeye Trevally - no size limit specified)
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(std::make_unique<VertebrateCreature>(
        "Bigeye Trevally", 20.0f, false)); // Size doesn't matter here
  }
  // Total Trevallies = 10. Should pass.
  ASSERT_TRUE(checker.validate(testBag));
}

TEST(BagCheckerTest, ValidateFailsWhenTotalTunaLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 6 small Tuna (< 90cm) - Exceeds total limit of 5
  for (int i = 0; i < 6; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Yellowfin Tuna", 80.0f, false));
  }
  ASSERT_FALSE(checker.validate(testBag));
}

TEST(BagCheckerTest, ValidateFailsWhenLargeTunaLimitExceeded) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 3 large Tuna (>= 90cm) - Exceeds large limit of 2
  for (int i = 0; i < 3; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Yellowfin Tuna", 100.0f, false));
  }
  ASSERT_FALSE(checker.validate(testBag));
}

TEST(BagCheckerTest, ValidateFailsWhenLargeTunaLimitExceededMix) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 3 large Tuna and 2 small Tuna (Total 5 is okay, but large limit 2
  // exceeded)
  for (int i = 0; i < 3; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Bigeye Tuna", 100.0f, false));
  }
  for (int i = 0; i < 2; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Albacore Tuna", 80.0f, false));
  }
  ASSERT_FALSE(checker.validate(testBag));
}

TEST(BagCheckerTest, ValidatePassesWithValidTunaMix) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 2 large Tuna, 3 small Tuna (Total 5, Large 2 - OK)
  for (int i = 0; i < 2; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Longtail Tuna", 95.0f, false));
  }
  for (int i = 0; i < 3; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Yellowfin Tuna", 70.0f, false));
  }
  ASSERT_TRUE(checker.validate(testBag));
}

TEST(BagCheckerTest, ValidatePassesWithOnlySmallTuna) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 5 small Tuna (Total 5, Large 0 - OK)
  for (int i = 0; i < 5; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Albacore Tuna", 85.0f, false));
  }
  ASSERT_TRUE(checker.validate(testBag));
}

TEST(BagCheckerTest, ValidatePassesWithOnlyLargeTuna) {
  VertebrateChecker v_checker;
  InvertebrateChecker i_checker;
  BagChecker checker(v_checker, i_checker);
  Bag testBag;
  // Add 2 large Tuna (Total 2, Large 2 - OK)
  for (int i = 0; i < 2; ++i) {
    testBag.addCreature(
        std::make_unique<VertebrateCreature>("Bigeye Tuna", 110.0f, false));
  }
  ASSERT_TRUE(checker.validate(testBag));
}