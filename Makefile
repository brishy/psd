
#compiler stuff
CXX = g++
CPPFLAGS = -std=c++17 -Wall
#project includes
INCFLAGS = -Iinclude
#gtest base dir
GTEST_BASE_DIR = googletest/googletest
#gtest includes (separate from project includes)
GTEST_INCFLAGS = -Igoogletest/googletest/include
#linker flags (need -pthread for gtest)
LDFLAGS = -pthread

#dirs
OBJDIR = obj
SRCDIR = src
TESTDIR = tests

#app source files (*.cpp in src/)
SRCS = $(wildcard $(SRCDIR)/*.cpp)
#app object files (*.o in obj/)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

#app objects needed for linking tests (everything EXCEPT main.o)
APP_OBJS_FOR_TESTS = $(filter-out $(OBJDIR)/main.o, $(OBJS))

#test source files (*_test.cpp in tests/)
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
#test object files (*.o in obj/)
TEST_OBJS = $(patsubst $(TESTDIR)/%.cpp,$(OBJDIR)/%.o,$(TEST_SRCS))

#source files (just the main ones needed)
GTEST_ALL_SRC = $(GTEST_BASE_DIR)/src/gtest-all.cc
GTEST_MAIN_SRC = $(GTEST_BASE_DIR)/src/gtest_main.cc
#object files
GTEST_ALL_OBJ = $(OBJDIR)/gtest-all.o
GTEST_MAIN_OBJ = $(OBJDIR)/gtest_main.o

#exe names
MAIN_TARGET = main
TEST_TARGET = run_tests

#build main app if just 'make' is run
all: $(MAIN_TARGET)

#rule for main app exe (links all app objects)
$(MAIN_TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(INCFLAGS) $^ $(LDFLAGS) -o $@

#rule for test exe - needs test objs, app objs (not main.o), gtest objs
#fix: use APP_OBJS_FOR_TESTS here so main() isn't linked twice
$(TEST_TARGET): $(TEST_OBJS) $(APP_OBJS_FOR_TESTS) $(GTEST_ALL_OBJ) $(GTEST_MAIN_OBJ)
	$(CXX) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

#rule to run tests (builds run_tests first if needed)
test: $(TEST_TARGET)
	@echo "Running tests..." #use @ to not echo the echo command itself
	./$(TEST_TARGET)

#using hardcoded path for gtest src because wildcard is messy
$(GTEST_MAIN_OBJ): $(GTEST_MAIN_SRC) #hardcoded prerequisite path
	@mkdir -p $(OBJDIR)
	#compile gtest main obj
	$(CXX) $(CPPFLAGS) $(GTEST_INCFLAGS) -I$(GTEST_BASE_DIR) -c $< -o $@

#compile gtest-all.cc (needed for linking tests)
#using hardcoded path for gtest src
$(GTEST_ALL_OBJ): $(GTEST_ALL_SRC) #hardcoded prerequisite path
	@mkdir -p $(OBJDIR)
	#compile gtest lib obj
	$(CXX) $(CPPFLAGS) $(GTEST_INCFLAGS) -I$(GTEST_BASE_DIR) -c $< -o $@

#compile test sources (*_test.cpp -> obj/*.o)
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	#compile the test file
	$(CXX) $(CPPFLAGS) $(GTEST_INCFLAGS) $(INCFLAGS) -c $< -o $@

#compile app sources (*.cpp -> obj/*.o)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	#compile the app source file
	$(CXX) $(CPPFLAGS) $(INCFLAGS) -c $< -o $@

#clean up build files
clean:
	@echo "Cleaning build artifacts..." #use @ here too
	rm -rf $(OBJDIR) $(MAIN_TARGET) $(TEST_TARGET)

#phony targets don't represent files
.PHONY: all test clean