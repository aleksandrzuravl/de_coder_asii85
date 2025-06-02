CXX := g++
CXXFLAGS := -Werror -Wpedantic -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS := -pthread
GTEST_LIBS := -lgtest -lgtest_main

BUILDDIR := build
BINDIR := bin
SRCDIR := .

TARGET := $(BINDIR)/Start
TEST_TARGET := $(BINDIR)/test

SRCS := $(wildcard $(SRCDIR)/*.cpp)
MAIN_SRC := $(SRCDIR)/main.cpp
TEST_SRC := $(SRCDIR)/test.cpp

OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(TEST_SRC),$(SRCS)))
TEST_OBJS := $(filter-out $(BUILDDIR)/main.o,$(OBJS)) $(BUILDDIR)/test.o

all: $(BUILDDIR) $(BINDIR) $(TARGET)

test: $(BUILDDIR) $(BINDIR) $(TEST_TARGET)
	@$(TEST_TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(GTEST_LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR) $(BINDIR):
	@mkdir -p $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean test
