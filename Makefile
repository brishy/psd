# Compiler and flags
CXX = g++
# NOTE: Ensure -I flags use correct paths, no trailing spaces
CPPFLAGS = -std=c++17 -Wall
INCFLAGS = -Iinclude # Include path for your project headers
GTEST_BASE_DIR = googletest/googletest # Base gtest dir relative to project root
GTEST_INCFLAGS = -Igoogletest/googletest/include
LDFLAGS = -pthread # Flags for linking (like pthread for gtest)

# Directories
OBJDIR = obj
SRCDIR = src
TESTDIR = tests

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Add this line right after the OBJS definition
APP_OBJS_FOR_TESTS = $(filter-out $(OBJDIR)/main.o, $(OBJS))

# Test source files
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS = $(patsubst $(TESTDIR)/%.cpp,$(OBJDIR)/%.o,$(TEST_SRCS))

# Google Test source file paths
GTEST_ALL_SRC = $(GTEST_BASE_DIR)/src/gtest-all.cc
GTEST_MAIN_SRC = $(GTEST_BASE_DIR)/src/gtest_main.cc
# Google Test object file paths
GTEST_ALL_OBJ = $(OBJDIR)/gtest-all.o
GTEST_MAIN_OBJ = $(OBJDIR)/gtest_main.o

# Target executable names
MAIN_TARGET = main
TEST_TARGET = run_tests

# Default target
all: $(MAIN_TARGET)

# Rule for the main application executable
$(MAIN_TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(INCFLAGS) $^ $(LDFLAGS) -o $@

# Rule for the test executable - depends on test objs, app objs (EXCEPT main.o), and gtest objs
# --- FIX: Changed $(OBJS) dependency to $(APP_OBJS_FOR_TESTS) ---
$(TEST_TARGET): $(TEST_OBJS) $(APP_OBJS_FOR_TESTS) $(GTEST_ALL_OBJ) $(GTEST_MAIN_OBJ)
# --- End FIX ---
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@ # <-- Linking command itself is okay

# Rule to run tests (depends on the test executable being built)
test: $(TEST_TARGET)
	echo "Running tests..."
	./$(TEST_TARGET)

# --- Rule to compile Google Test main ---
# --- Hardcoded Prerequisite and Diagnostics ---
$(GTEST_MAIN_OBJ): googletest/googletest/src/gtest_main.cc # <-- HARDCODED Prerequisite Path
	@mkdir -p $(OBJDIR)
	@echo "--- Compiling Google Test Main ---"
	@echo "Target ($@): $@"
	@echo "Prerequisite ($<): $<" # See what make thinks the prerequisite is
	@echo "Using CXX           : $(CXX)"
	@echo "Using CPPFLAGS      : $(CPPFLAGS)"
	@echo "Using GTEST_INCFLAGS: $(GTEST_INCFLAGS)"
	@echo "Using Include Path  : -I$(GTEST_BASE_DIR)"
	@echo "--- Running Actual Command: ---"
	# Compile using the automatic variable $< (which should be the hardcoded prerequisite)
	$(CXX) $(CPPFLAGS) $(GTEST_INCFLAGS) -I$(GTEST_BASE_DIR) -c $< -o $@

# --- Rule to compile Google Test library (gtest-all.cc) --- (NEEDS TO BE PRESENT)
# --- Using Hardcoded Prerequisite and Diagnostics ---
$(GTEST_ALL_OBJ): googletest/googletest/src/gtest-all.cc # <-- HARDCODED Prerequisite Path
	@mkdir -p $(OBJDIR)
	@echo "--- Compiling Google Test Library ---"
	@echo "Target ($@): $@"
	@echo "Prerequisite ($<): $<" # See what make thinks the prerequisite is
	@echo "Using CXX           : $(CXX)"
	@echo "Using CPPFLAGS      : $(CPPFLAGS)"
	@echo "Using GTEST_INCFLAGS: $(GTEST_INCFLAGS)"
	@echo "Using Include Path  : -I$(GTEST_BASE_DIR)"
	@echo "--- Running Actual Command: ---"
	# Compile using the automatic variable $< (which should be the hardcoded prerequisite)
	$(CXX) $(CPPFLAGS) $(GTEST_INCFLAGS) -I$(GTEST_BASE_DIR) -c $< -o $@

# --- Rule to compile test source files (Reverted back to using variables) ---
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@echo "--- Compiling Test File: $< ---" # Keep diagnostics for now
	@echo "Using GTEST_INCFLAGS: $(GTEST_INCFLAGS)"
	# Original Compile Command (using variables):
	$(CXX) $(CPPFLAGS) $(GTEST_INCFLAGS) $(INCFLAGS) -c $< -o $@

# Rule to compile main application source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	# Optional echo for debugging source compilation:
	# @echo "Compiling source file $<"
	$(CXX) $(CPPFLAGS) $(INCFLAGS) -c $< -o $@

# Clean rule
clean:
	echo "Cleaning build artifacts..."
	rm -rf $(OBJDIR) $(MAIN_TARGET) $(TEST_TARGET)

.PHONY: all test clean