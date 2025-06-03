CXX := g++
CXXFLAGS := -Werror -Wpedantic -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS := -pthread
GTEST_LIBS := -lgtest -lgtest_main -lgmock

BUILDDIR := build
BINDIR := bin
SRCDIR := src
TESTDIR := test

TARGET := $(BINDIR)/Start
MAIN_TEST_TARGET := $(BINDIR)/codec_test
RANDOM_TEST_TARGET := $(BINDIR)/random_test

SRCS := $(wildcard $(SRCDIR)/*.cpp)
MAIN_SRC := $(SRCDIR)/main.cpp
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(MAIN_SRC),$(SRCS)))

TEST_SRCS := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS := $(filter-out $(BUILDDIR)/main.o,$(OBJS)) \
             $(patsubst $(TESTDIR)/%.cpp,$(BUILDDIR)/%.o,$(TEST_SRCS))

.PHONY: all test random_test clean

all: $(BUILDDIR) $(BINDIR) $(TARGET)

test: $(MAIN_TEST_TARGET)
	@$(MAIN_TEST_TARGET)

random_test: $(RANDOM_TEST_TARGET)
	@$(RANDOM_TEST_TARGET)

$(TARGET): $(OBJS) $(BUILDDIR)/main.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(MAIN_TEST_TARGET): $(filter-out $(BUILDDIR)/random_test.o,$(TEST_OBJS))
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(GTEST_LIBS)

$(RANDOM_TEST_TARGET): $(filter-out $(BUILDDIR)/test.o,$(TEST_OBJS))
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(GTEST_LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(TESTDIR) -c $< -o $@

$(BUILDDIR) $(BINDIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
