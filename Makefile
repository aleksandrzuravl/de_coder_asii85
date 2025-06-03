CXX := g++
CXXFLAGS := -Werror -Wpedantic -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS := -pthread
GTEST_LIBS := -lgtest -lgtest_main -lgmock

BUILDDIR := build
BINDIR := bin
SRCDIR := src
TESTDIR := test

TARGET := $(BINDIR)/Start
TEST_TARGET := $(BINDIR)/codec_test

SRCS := $(wildcard $(SRCDIR)/*.cpp)
MAIN_SRC := $(SRCDIR)/main.cpp
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(MAIN_SRC),$(SRCS)))

TEST_SRC := $(TESTDIR)/test.cpp
TEST_OBJS := $(filter-out $(BUILDDIR)/main.o,$(OBJS)) $(BUILDDIR)/test.o

.PHONY: all test clean

all: $(BUILDDIR) $(BINDIR) $(TARGET)

test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(TARGET): $(OBJS) $(BUILDDIR)/main.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS) $(BUILDDIR)/test.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(GTEST_LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/test.o: $(TESTDIR)/test.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(TESTDIR) -c $< -o $@

$(BUILDDIR) $(BINDIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
